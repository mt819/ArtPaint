/*
 * Copyright 2003, Heikki Suhonen
 * Copyright 2009, Karsten Heimrich
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Heikki Suhonen <heikki.suhonen@gmail.com>
 *		Karsten Heimrich <host.haiku@gmx.de>
 *		Dale Cieslak <dcieslak@yahoo.com>
 *
 */

#include "RectangleTool.h"

#include "BitmapDrawer.h"
#include "Cursors.h"
#include "HSPolygon.h"
#include "Image.h"
#include "ImageView.h"
#include "PaintApplication.h"
#include "ToolScript.h"
#include "UtilityClasses.h"


#include <Catalog.h>
#include <CheckBox.h>
#include <GroupLayoutBuilder.h>
#include <RadioButton.h>
#include <SeparatorView.h>
#include <Window.h>


#include <math.h>
#include <stdio.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Tools"


RectangleTool::RectangleTool()
	: DrawingTool(B_TRANSLATE("Rectangle tool"), "r",
		RECTANGLE_TOOL)
{
	fOptions = FILL_ENABLED_OPTION | SIZE_OPTION | SHAPE_OPTION
		| ROTATION_ENABLED_OPTION | ANTI_ALIASING_LEVEL_OPTION;
	fOptionsCount = 5;

	SetOption(FILL_ENABLED_OPTION, B_CONTROL_OFF);
	SetOption(SIZE_OPTION, 1);
	SetOption(SHAPE_OPTION, HS_CORNER_TO_CORNER);
	SetOption(ROTATION_ENABLED_OPTION, B_CONTROL_OFF);
	SetOption(ANTI_ALIASING_LEVEL_OPTION, B_CONTROL_OFF);
}


RectangleTool::~RectangleTool()
{
}


ToolScript*
RectangleTool::UseTool(ImageView *view, uint32 buttons, BPoint point,
	BPoint view_point)
{
	// Wait for the last_updated_region to become empty
	while (LastUpdatedRect().IsValid())
		snooze(50000);

	BWindow *window = view->Window();
	drawing_mode old_mode;
	BBitmap *bitmap = view->ReturnImage()->ReturnActiveBitmap();
//	BView *bitmap_view = view->getBufferView();

	ToolScript *the_script = new ToolScript(Type(), fToolSettings,
		((PaintApplication*)be_app)->Color(true));
	Selection *selection = view->GetSelection();

	bool draw_rectangle = true;

	if (window != NULL) {
		BitmapDrawer *drawer = new BitmapDrawer(bitmap);

		BPoint original_point;
		BRect bitmap_rect,old_rect,new_rect;
		window->Lock();
		rgb_color old_color = view->HighColor();
		old_mode = view->DrawingMode();
		view->SetDrawingMode(B_OP_INVERT);
		window->Unlock();
		bool use_fg_color = true;
		if (buttons == B_SECONDARY_MOUSE_BUTTON)
			use_fg_color = false;

		rgb_color c = ((PaintApplication*)be_app)->Color(use_fg_color);
		original_point = point;
		bitmap_rect = BRect(point,point);
		old_rect = new_rect = view->convertBitmapRectToView(bitmap_rect);
		window->Lock();
		view->SetHighColor(c);
		view->StrokeRect(new_rect, B_SOLID_HIGH);
		window->Unlock();

		while (buttons) {
			window->Lock();
			view->getCoords(&point,&buttons,&view_point);
			window->Unlock();
			bitmap_rect = MakeRectFromPoints(original_point, point);
			if (modifiers() & B_SHIFT_KEY) {
				// Make the rectangle square.
				float max_distance = max_c(bitmap_rect.Height(),bitmap_rect.Width());
				if (original_point.x == bitmap_rect.left)
					bitmap_rect.right = bitmap_rect.left + max_distance;
				else
					bitmap_rect.left = bitmap_rect.right - max_distance;

				if (original_point.y == bitmap_rect.top)
					bitmap_rect.bottom = bitmap_rect.top + max_distance;
				else
					bitmap_rect.top = bitmap_rect.bottom - max_distance;
			}
			if (modifiers() & B_COMMAND_KEY) {
				// Make the the rectangle original corner be at the center of
				// new rectangle.
				float y_distance = bitmap_rect.Height();
				float x_distance = bitmap_rect.Width();

				if (bitmap_rect.left == original_point.x)
					bitmap_rect.left = bitmap_rect.left - x_distance;
				else
					bitmap_rect.right = bitmap_rect.right + x_distance;

				if (bitmap_rect.top == original_point.y)
					bitmap_rect.top = bitmap_rect.top - y_distance;
				else
					bitmap_rect.bottom = bitmap_rect.bottom + y_distance;

			}
			new_rect = view->convertBitmapRectToView(bitmap_rect);

			if (view->LockLooper() == true) {
				if (old_rect != new_rect) {
					view->Draw(old_rect);
					view->StrokeRect(new_rect, B_SOLID_HIGH);
					old_rect = new_rect;
				}
				view->UnlockLooper();
			}
			snooze(20 * 1000);
		}

		HSPolygon *poly;
		BPoint *corners = new BPoint[4];
		corners[0] = bitmap_rect.LeftTop();
		corners[1] = bitmap_rect.RightTop();
		corners[2] = bitmap_rect.RightBottom();
		corners[3] = bitmap_rect.LeftBottom();
		poly = new HSPolygon(corners,4);

		if (GetCurrentValue(ROTATION_ENABLED_OPTION) == B_CONTROL_ON) {
			HSPolygon *view_poly;
			corners[0] = new_rect.LeftTop();
			corners[1] = new_rect.RightTop();
			corners[2] = new_rect.RightBottom();
			corners[3] = new_rect.LeftBottom();
			view_poly = new HSPolygon(corners,4);

			BPoint centroid = bitmap_rect.LeftTop() + bitmap_rect.RightTop()
				+ bitmap_rect.RightBottom() + bitmap_rect.LeftBottom();
			centroid.x /= 4;
			centroid.y /= 4;
			float new_angle,prev_angle;
			prev_angle = new_angle =0;
			bool continue_rotating = true;
			float original_angle;
			if (centroid.x != point.x) {
				original_angle = atan((centroid.y-point.y)
					/ (centroid.x-point.x)) * 180 /M_PI;
			} else {
				original_angle = 90;
			}

			while (continue_rotating) {
				if (is_clicks_data_valid) {
					continue_rotating = FALSE;
					is_clicks_data_valid = FALSE;
				}
				else if (is_keys_data_valid) {
					if (last_key_event_bytes[0] == B_ESCAPE) {
						continue_rotating = FALSE;
						draw_rectangle = FALSE;
					}
					is_keys_data_valid = FALSE;
				}
				else {
					// Here we should rotate the polygon
					window->Lock();
					if (new_angle != prev_angle) {
						BRect bbox = view_poly->BoundingBox();
						view->Draw(bbox);

//						poly->RotateAboutCenter(new_angle - prev_angle);
						view_poly->RotateAboutCenter(new_angle - prev_angle);

						BPolygon* draw_poly = view_poly->GetBPolygon();
						view->StrokePolygon(draw_poly);
						delete draw_poly;
						prev_angle = new_angle;
					}
					view->getCoords(&point,&buttons);
					window->Unlock();
					if (centroid.x != point.x) {
						new_angle = atan((centroid.y-point.y)
							/ (centroid.x-point.x)) * 180 / M_PI;
					} else {
						new_angle = 90;
					}
					new_angle -= original_angle;

					if (modifiers() & B_SHIFT_KEY)
						new_angle = SnapToAngle(22.5, new_angle);

					snooze(20 * 1000);
				}
			}
			poly->RotateAboutCenter(prev_angle);

			delete view_poly;
		}

		bitmap->Lock();
		bool fill = (GetCurrentValue(FILL_ENABLED_OPTION) == B_CONTROL_ON);
		bool anti_a = (GetCurrentValue(ANTI_ALIASING_LEVEL_OPTION) == B_CONTROL_ON);
		if (draw_rectangle == true) {
			if (poly->GetPointCount() == 4) {
				drawer->DrawRectanglePolygon(poly->GetPointList(),
					RGBColorToBGRA(c), fill, anti_a, selection);
			} else {
				drawer->DrawRectanglePolygon(corners, RGBColorToBGRA(c), fill,
					anti_a, selection);
			}
		}
		bitmap->Unlock();
		SetLastUpdatedRect(poly->BoundingBox().InsetByCopy(-1.0, -1.0));
		if (poly->GetPointCount() == 4) {
			BPoint *points = poly->GetPointList();
			for (int32 i=0;i<4;i++)
				the_script->AddPoint(points[i]);
		} else {
			for (int32 i=0;i<4;i++)
				the_script->AddPoint(corners[i]);
		}
		delete poly;
		delete[] corners;

		window->Lock();
		view->SetHighColor(old_color);
		view->SetDrawingMode(old_mode);
		view->UpdateImage(LastUpdatedRect());
		view->Sync();
		window->Unlock();

		delete drawer;

	}

	return the_script;
}


int32
RectangleTool::UseToolWithScript(ToolScript*, BBitmap*)
{
	return B_OK;
}


BView*
RectangleTool::ConfigView()
{
	return new RectangleToolConfigView(this);
}


const void*
RectangleTool::ToolCursor() const
{
	return HS_RECTANGLE_CURSOR;
}


const char*
RectangleTool::HelpString(bool isInUse) const
{
	return (isInUse
		? B_TRANSLATE("Drawing a rectangle.")
		: B_TRANSLATE("Rectangle: SHIFT for square, ALT for centered"));
}


// #pragma mark -- RectangleToolConfigView


RectangleToolConfigView::RectangleToolConfigView(DrawingTool* tool)
	: DrawingToolConfigView(tool)
{
	if (BLayout* layout = GetLayout()) {
		BMessage* message = new BMessage(OPTION_CHANGED);
		message->AddInt32("option", FILL_ENABLED_OPTION);
		message->AddInt32("value", 0x00000000);

		fFillRectangle =
			new BCheckBox(B_TRANSLATE("Fill rectangle"),
			message);

		message = new BMessage(OPTION_CHANGED);
		message->AddInt32("option", ROTATION_ENABLED_OPTION);
		message->AddInt32("value", 0x00000000);

		fRotation =
			new BCheckBox(B_TRANSLATE("Enable rotation"),
			message);

		message = new BMessage(OPTION_CHANGED);
		message->AddInt32("option", ANTI_ALIASING_LEVEL_OPTION);
		message->AddInt32("value", 0x00000000);

		fAntiAlias =
			new BCheckBox(B_TRANSLATE("Enable antialiasing"),
			message);

		BSeparatorView* view =
			new BSeparatorView(B_TRANSLATE("Mode"),
			B_HORIZONTAL, B_FANCY_BORDER, BAlignment(B_ALIGN_LEFT,
			B_ALIGN_VERTICAL_CENTER));
		view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

		layout->AddView(BGroupLayoutBuilder(B_VERTICAL, kWidgetSpacing)
			.AddGroup(B_VERTICAL, kWidgetSpacing)
				.Add(fFillRectangle)
				.SetInsets(kWidgetInset, 0.0, 0.0, 0.0)
			.End()
			.AddStrut(kWidgetSpacing)
			.Add(SeparatorView(B_TRANSLATE("Options")))
			.AddGroup(B_VERTICAL, kWidgetSpacing)
				.Add(fRotation)
				.Add(fAntiAlias)
				.SetInsets(kWidgetInset, 0.0, 0.0, 0.0)
			.End()
			.TopView()
		);

		if (tool->GetCurrentValue(FILL_ENABLED_OPTION) != B_CONTROL_OFF)
			fFillRectangle->SetValue(B_CONTROL_ON);

		if (tool->GetCurrentValue(ROTATION_ENABLED_OPTION) != B_CONTROL_OFF)
			fRotation->SetValue(B_CONTROL_ON);

		if (tool->GetCurrentValue(ANTI_ALIASING_LEVEL_OPTION) != B_CONTROL_OFF)
			fAntiAlias->SetValue(B_CONTROL_ON);
	}
}


void
RectangleToolConfigView::AttachedToWindow()
{
	DrawingToolConfigView::AttachedToWindow();

	fFillRectangle->SetTarget(this);
	fRotation->SetTarget(this);
	fAntiAlias->SetTarget(this);
}
