/*
 * Copyright 2003, Heikki Suhonen
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Heikki Suhonen <heikki.suhonen@gmail.com>
 *
 */
#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "ManipulatorInformer.h"
#include "ManipulatorSettings.h"
#include "WindowGUIManipulator.h"
#include "ThresholdView.h"

class	ThresholdManipulatorSettings : public ManipulatorSettings {
public:
		ThresholdManipulatorSettings()
			: ManipulatorSettings() {
			threshold = 127;
			mode = 0;
		}

		ThresholdManipulatorSettings(const ThresholdManipulatorSettings& s)
			: ManipulatorSettings() {
			threshold = s.threshold;
			mode = s.mode;
		}


		ThresholdManipulatorSettings& operator=(const ThresholdManipulatorSettings& s) {
			threshold = s.threshold;
			mode = s.mode;
			return *this;
		}


		bool operator==(ThresholdManipulatorSettings s) {
			return ((threshold == s.threshold) &&
					(mode == s.mode));
		}

		bool operator!=(ThresholdManipulatorSettings s) {
			return !(*this==s);
		}

int32		threshold;
int32		mode;
};


class ThresholdManipulatorView;

class ThresholdManipulator : public WindowGUIManipulator {
			BBitmap	*preview_bitmap;
			BBitmap	*copy_of_the_preview_bitmap;

			int32	lowest_available_quality;
			int32	highest_available_quality;
			int32	last_calculated_resolution;

			ThresholdManipulatorSettings	settings;
			ThresholdManipulatorSettings	previous_settings;

			ThresholdManipulatorView		*config_view;


			// The next attributes will be used by the thread_function.
			int32	number_of_threads;
			int32	current_resolution;

			ThresholdManipulatorSettings	current_settings;

			Selection	*selection;

			BBitmap		*source_bitmap;
			BBitmap		*target_bitmap;
			BStatusBar	*progress_bar;

			void	start_threads();

	static	int32	thread_entry(void*);
			int32	thread_function(int32);


			rgb_color	light_color;
			rgb_color	dark_color;

public:
			ThresholdManipulator(BBitmap*,ManipulatorInformer*);
			~ThresholdManipulator();

void		MouseDown(BPoint,uint32 buttons,BView*,bool);
int32		PreviewBitmap(bool full_quality = FALSE, BRegion* =NULL);
BBitmap*	ManipulateBitmap(ManipulatorSettings*, BBitmap*, BStatusBar*);
void		Reset();
void		SetPreviewBitmap(BBitmap*);
const char*	ReturnHelpString();
const char*	ReturnName();

ManipulatorSettings*	ReturnSettings();

BView*		MakeConfigurationView(const BMessenger& target);

void		ChangeSettings(ManipulatorSettings*);

status_t	WriteSettings(BNode *node);
status_t	ReadSettings(BNode *node);
void		SetSelection(Selection* new_selection)
				{ selection = new_selection; };
};



#define	THRESHOLD_ADJUSTED				'Thad'
#define	THRESHOLD_ADJUSTING_FINISHED	'Thaf'

class ThresholdManipulatorView : public WindowGUIManipulatorView {
		BMessenger						target;
		ThresholdManipulator			*manipulator;
		ThresholdManipulatorSettings	settings;

		ThresholdView					*threshold_control;


		bool							started_adjusting;
public:
		ThresholdManipulatorView(ThresholdManipulator*, const BMessenger& target);
		~ThresholdManipulatorView();

void	AttachedToWindow();
void	MessageReceived(BMessage*);
void	ChangeSettings(ManipulatorSettings*);
void	SetBitmap(BBitmap*);
};

#endif



