/*
 * Copyright 2003, Heikki Suhonen
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Heikki Suhonen <heikki.suhonen@gmail.com>
 *
 */
#include <Bitmap.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <Node.h>
#include <StatusBar.h>
#include <Slider.h>
#include <string.h>
#include <Window.h>

#include "AddOns.h"
#include "Contrast.h"
#include "ManipulatorInformer.h"
#include "Selection.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "AddOns_Contrast"


#ifdef __cplusplus
extern "C" {
#endif
	char name[255] = B_TRANSLATE_MARK("Contrast" B_UTF8_ELLIPSIS);
	char menu_help_string[255] = B_TRANSLATE_MARK("Adjusts the contrast.");
	int32 add_on_api_version = ADD_ON_API_VERSION;
	add_on_types add_on_type = COLOR_ADD_ON;
#ifdef __cplusplus
}
#endif


Manipulator* instantiate_add_on(BBitmap *bm,ManipulatorInformer *i)
{
	delete i;
	return new ContrastManipulator(bm);
}



ContrastManipulator::ContrastManipulator(BBitmap *bm)
		: WindowGUIManipulator(),
		selection(NULL)
{
	preview_bitmap = NULL;
	config_view = NULL;
	copy_of_the_preview_bitmap = NULL;

	previous_settings.contrast = settings.contrast + 1;

	SetPreviewBitmap(bm);
}


ContrastManipulator::~ContrastManipulator()
{
	delete copy_of_the_preview_bitmap;
	delete config_view;
}


BBitmap* ContrastManipulator::ManipulateBitmap(ManipulatorSettings* set,
	BBitmap* original, BStatusBar* status_bar)
{
	ContrastManipulatorSettings* new_settings = dynamic_cast<ContrastManipulatorSettings*>(set);

	if (new_settings == NULL)
		return NULL;

	if (original == NULL)
		return NULL;

	if (original == preview_bitmap) {
		if ((*new_settings == previous_settings) && (last_calculated_resolution <= 1))
			return original;

		source_bitmap = copy_of_the_preview_bitmap;
		target_bitmap = original;
	}
	else {
		source_bitmap = original;
		target_bitmap = new BBitmap(original->Bounds(),B_RGB32,FALSE);
	}


	current_resolution = 1;
	current_settings = *new_settings;
	progress_bar = status_bar;
	current_average_luminance = CalculateAverageLuminance(source_bitmap);

	start_threads();

	return target_bitmap;
}

int32 ContrastManipulator::PreviewBitmap(bool full_quality,\
	BRegion* updated_region)
{
	progress_bar = NULL;
	if (settings == previous_settings ) {
		if ((last_calculated_resolution != highest_available_quality) && (last_calculated_resolution > 0))
			last_calculated_resolution = max_c(highest_available_quality,floor(last_calculated_resolution/2.0));
		else
			last_calculated_resolution = 0;
	}
	else
		last_calculated_resolution = lowest_available_quality;

	if (full_quality) {
		last_calculated_resolution = min_c(1,last_calculated_resolution);
	}
	previous_settings = settings;

	if (last_calculated_resolution > 0) {
		current_resolution = last_calculated_resolution;
		updated_region->Set(preview_bitmap->Bounds());

		target_bitmap = preview_bitmap;
		source_bitmap = copy_of_the_preview_bitmap;
		current_settings = settings;

		start_threads();
	}

	return last_calculated_resolution;
}


uint8 ContrastManipulator::CalculateAverageLuminance(BBitmap *bitmap)
{
	uint32 *bits = (uint32*)bitmap->Bits();
	uint32 bits_length = bitmap->BitsLength()/4;

	double luminance_sum = 0;

	union {
		uint8 bytes[4];
		uint32 word;
	} color;

	for (int32 i=0;i<bits_length;i++) {
		color.word = *bits++;
		luminance_sum += 0.299 * color.bytes[0] + 0.587 * color.bytes[1] + 0.144 * color.bytes[2];
	}

	luminance_sum = luminance_sum / bits_length;

	uint8 sum = (uint8)luminance_sum;

	return sum;
}

void ContrastManipulator::start_threads()
{
	number_of_threads = GetSystemCpuCount();

	thread_id *threads = new thread_id[number_of_threads];

	for (int32 i=0;i<number_of_threads;i++) {
		threads[i] = spawn_thread(thread_entry,"contrast_thread",B_NORMAL_PRIORITY,this);
		resume_thread(threads[i]);
		send_data(threads[i],i,NULL,0);
	}

	for (int32 i=0;i<number_of_threads;i++) {
		int32 return_value;
		wait_for_thread(threads[i],&return_value);
	}

	delete[] threads;
}

int32 ContrastManipulator::thread_entry(void *data)
{
	int32 thread_number;
	thread_number = receive_data(NULL,NULL,0);

	ContrastManipulator *this_pointer = (ContrastManipulator*)data;

	return this_pointer->thread_function(thread_number);
}


int32 ContrastManipulator::thread_function(int32 thread_number)
{
	// This function interpolates the image with a degenerate version,
	// which in this case is the average luminance. The luminance image is not actually
	// used, but only implied. This function does not touch the alpha-channel.

	int32 step = current_resolution;
	uint32 contrast = settings.contrast;

	BWindow *progress_bar_window = NULL;
	if (progress_bar != NULL)
		progress_bar_window = progress_bar->Window();


	uint32 *source_bits = (uint32*)source_bitmap->Bits();
	uint32 *target_bits = (uint32*)target_bitmap->Bits();
	int32 source_bpr = source_bitmap->BytesPerRow()/4;
	int32 target_bpr = target_bitmap->BytesPerRow()/4;

	// This union must be used to guarantee endianness compatibility.
	union {
		uint8 bytes[4];
		uint32 word;
	} color;

	float coeff = current_settings.contrast / 100.0;
	float one_minus_coeff = 1.0 - coeff;
	int32 luminance_factor = current_average_luminance*one_minus_coeff;;

	uint8 luminance_values[256];
	for (int32 i=0;i<256;i++) {
		luminance_values[i] = max_c(0,min_c(255,i * coeff + luminance_factor));
	}

	if (selection->IsEmpty()) {
		// Here handle the whole image.
		int32 left = target_bitmap->Bounds().left;
		int32 right = target_bitmap->Bounds().right;
		int32 top = target_bitmap->Bounds().top;
		int32 bottom = target_bitmap->Bounds().bottom;

		float height = bottom - top;
		top = height/number_of_threads*thread_number;
		top = ceil(top/(float)step);
		top *= step;
		bottom = min_c(bottom,top + (height+1)/number_of_threads);
		int32 update_interval = 10;
		float update_amount = 100.0/(bottom-top)*update_interval/(float)number_of_threads;
		float missed_update = 0;

		// Loop through all pixels in original.
		uint32 sum;
		contrast *= 3;
		for (int32 y=top;y<=bottom;y+=step) {
			int32 y_times_source_bpr = y*source_bpr;
			int32 y_times_target_bpr = y*target_bpr;
			for (int32 x=left;x<=right;x+=step) {
				color.word = *(source_bits + x + y_times_source_bpr);
				color.bytes[0] = luminance_values[color.bytes[0]];
				color.bytes[1] = luminance_values[color.bytes[1]];
				color.bytes[2] = luminance_values[color.bytes[2]];
				*(target_bits + x + y_times_target_bpr) = color.word;
			}

			// Update the status-bar
			if ( ((y % update_interval) == 0) && (progress_bar_window != NULL) && (progress_bar_window->LockWithTimeout(0) == B_OK) ) {
				progress_bar->Update(update_amount+missed_update);
				progress_bar_window->Unlock();
				missed_update = 0;
			}
			else if ((y % update_interval) == 0) {
				missed_update += update_amount;
			}
		}
	}
	else {
		// Here handle only those pixels for which selection->ContainsPoint(x,y) is true.
		BRect rect = selection->GetBoundingRect();

		int32 left = rect.left;
		int32 right = rect.right;
		int32 top = rect.top;
		int32 bottom = rect.bottom;

		float height = bottom - top;
		top += height/number_of_threads*thread_number;
		top *= step;
		top /= step;

		bottom = min_c(bottom,top + (height+1)/number_of_threads);

		int32 update_interval = 10;
		float update_amount = 100.0/(bottom-top)*update_interval/(float)number_of_threads;

		// Loop through all pixels in original.
		for (int32 y=top;y<=bottom;y+=step) {
			int32 y_times_source_bpr = y*source_bpr;
			int32 y_times_target_bpr = y*target_bpr;
			for (int32 x=left;x<=right;x+=step) {
				if (selection->ContainsPoint(x,y)) {
					color.word = *(source_bits + x + y_times_source_bpr);
					color.bytes[0] = luminance_values[color.bytes[0]];
					color.bytes[1] = luminance_values[color.bytes[1]];
					color.bytes[2] = luminance_values[color.bytes[2]];
					*(target_bits + x + y_times_target_bpr) = color.word;
				}
			}

			// Update the status-bar
			if ( ((y % update_interval) == 0) && (progress_bar_window != NULL) && (progress_bar_window->LockWithTimeout(0) == B_OK) ) {
				progress_bar->Update(update_amount);
				progress_bar_window->Unlock();
			}
		}
	}

	return B_OK;
}


void ContrastManipulator::MouseDown(BPoint point,uint32,BView*,bool first_click)
{
	// This function does nothing in ContrastManipulator.
}


void ContrastManipulator::SetPreviewBitmap(BBitmap *bm)
{
	if (preview_bitmap != bm) {
		delete copy_of_the_preview_bitmap;
		if (bm != NULL) {
			preview_bitmap = bm;
			copy_of_the_preview_bitmap = DuplicateBitmap(bm,0);
		}
		else {
			preview_bitmap = NULL;
			copy_of_the_preview_bitmap = NULL;
		}
	}

	if (preview_bitmap != NULL) {
		// Let's select a resolution that can handle all the pixels at least
		// 10 times in a second while assuming that one pixel calculation takes
		// about 50 CPU cycles.
		double speed = GetSystemClockSpeed() / (10*50);
		BRect bounds = preview_bitmap->Bounds();
		float num_pixels = (bounds.Width()+1) * (bounds.Height() + 1);
		lowest_available_quality = 1;
		while ((num_pixels/lowest_available_quality/lowest_available_quality) > speed)
			lowest_available_quality *= 2;

		lowest_available_quality = min_c(lowest_available_quality,16);
		highest_available_quality = max_c(lowest_available_quality/2,1);

		current_average_luminance = CalculateAverageLuminance(preview_bitmap);
	}
	else {
		lowest_available_quality = 1;
		highest_available_quality = 1;
	}
	last_calculated_resolution = lowest_available_quality;
}


void ContrastManipulator::Reset()
{
	if (copy_of_the_preview_bitmap != NULL) {
		// memcpy seems to be about 10-15% faster that copying with a loop.
		uint32 *source = (uint32*)copy_of_the_preview_bitmap->Bits();
		uint32 *target = (uint32*)preview_bitmap->Bits();
		uint32 bits_length = preview_bitmap->BitsLength();

		memcpy(target,source,bits_length);
	}
}

BView* ContrastManipulator::MakeConfigurationView(const BMessenger& target)
{
	if (config_view == NULL) {
		config_view = new ContrastManipulatorView(this,target);
		config_view->ChangeSettings(&settings);
	}

	return config_view;
}


ManipulatorSettings* ContrastManipulator::ReturnSettings()
{
	return new ContrastManipulatorSettings(settings);
}

void ContrastManipulator::ChangeSettings(ManipulatorSettings *s)
{
	ContrastManipulatorSettings *new_settings;
	new_settings = dynamic_cast<ContrastManipulatorSettings*>(s);

	if (new_settings != NULL) {
		settings = *new_settings;
	}
}

const char* ContrastManipulator::ReturnName()
{
	return B_TRANSLATE("Contrast");
}

const char* ContrastManipulator::ReturnHelpString()
{
	return B_TRANSLATE("Adjusts the contrast.");
}




// -------------------------------------
ContrastManipulatorView::ContrastManipulatorView(ContrastManipulator *manip,
		const BMessenger& t)
	: WindowGUIManipulatorView()
{
	target = t;
	manipulator = manip;
	started_adjusting = FALSE;

	contrast_slider = new BSlider("contrast_slider", B_TRANSLATE("Contrast:"),
		new BMessage(CONTRAST_ADJUSTING_FINISHED), 0, 255, B_HORIZONTAL,
		B_TRIANGLE_THUMB);
	contrast_slider->SetModificationMessage(new BMessage(CONTRAST_ADJUSTED));
	contrast_slider->SetLimitLabels(B_TRANSLATE("Low"), B_TRANSLATE("High"));
	contrast_slider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	contrast_slider->SetHashMarkCount(11);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(contrast_slider)
		.SetInsets(B_USE_SMALL_INSETS)
		.End();
}

ContrastManipulatorView::~ContrastManipulatorView()
{
}

void ContrastManipulatorView::AttachedToWindow()
{
	WindowGUIManipulatorView::AttachedToWindow();
	contrast_slider->SetTarget(BMessenger(this));
}

void ContrastManipulatorView::AllAttached()
{
	contrast_slider->SetValue(settings.contrast);
}

void ContrastManipulatorView::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case CONTRAST_ADJUSTED:
			settings.contrast = contrast_slider->Value();
			manipulator->ChangeSettings(&settings);
			if (!started_adjusting) {
				target.SendMessage(HS_MANIPULATOR_ADJUSTING_STARTED);
				started_adjusting = TRUE;
			}
			break;

		case CONTRAST_ADJUSTING_FINISHED:
			started_adjusting = FALSE;
			settings.contrast = contrast_slider->Value();
			manipulator->ChangeSettings(&settings);
			target.SendMessage(HS_MANIPULATOR_ADJUSTING_FINISHED);
			break;

		default:
			WindowGUIManipulatorView::MessageReceived(message);
			break;
	}
}


void ContrastManipulatorView::ChangeSettings(ManipulatorSettings *set)
{
	ContrastManipulatorSettings *new_settings = dynamic_cast<ContrastManipulatorSettings*>(set);

	if (set != NULL) {
		settings = *new_settings;

		BWindow *window = Window();
		if (window != NULL) {
			window->Lock();
			contrast_slider->SetValue(settings.contrast);
			window->Unlock();
		}
	}
}
