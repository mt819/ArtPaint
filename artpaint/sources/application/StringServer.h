/*
 * Copyright 2003, Heikki Suhonen
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Heikki Suhonen <heikki.suhonen@gmail.com>
 *
 */
#ifndef STRING_SERVER_H
#define	STRING_SERVER_H

#include <SupportDefs.h>

enum languages {
	ENGLISH_LANGUAGE,
	FINNISH_LANGUAGE,
	GERMAN_LANGUAGE,
	FRENCH_LANGUAGE
};

enum string_id {
	SEPARATOR = -1,
	ABOUT_ARTPAINT_STRING = 0,
	ABOUT_HELP_STRING,
	ACTIVE_LAYER_STRING,
	ADD_AREA_STRING,
	ADD_LAYER_HELP_STRING,
	ADD_LAYER_BEHIND_STRING,
	ADD_LAYER_IN_FRONT_STRING,
	ADD_LAYER_STRING,
	ADD_ONS_STRING,
	ADJUSTABLE_STRING,
	ADJUSTABLE_WIDTH_STRING,
	AIRBRUSH_STRING,
	ALL_LAYERS_COPY_HELP_STRING,
	ALL_LAYERS_CUT_HELP_STRING,
	ALL_LAYERS_STRING,
	ALPHA_STRING,
	APPLY_CHANGES_STRING,
	BACKGROUND_STRING,
	BLUE_STRING,
	BOTTOM_STRING,
	BRUSH_STRING,
	BRUSH_WINDOW_STRING,
	BRUSHES_STRING,
	CANCEL_STRING,
	CANNOT_CREATE_IMAGE_STRING,
	CANVAS_STRING,
	CENTER_TO_CORNER_STRING,
	CHANGES_TAKE_EFFECT_STRING,
	CHANGE_TRANSPARENCY_HELP_STRING,
	CHANGE_TRANSPARENCY_STRING,
	CLEAR_CANVAS_HELP_STRING,
	CLEAR_CANVAS_STRING,
	CLEAR_LAYER_HELP_STRING,
	CLEAR_LAYER_STRING,
	CLEAR_SELECTION_HELP_STRING,
	CLEAR_SELECTION_STRING,
	CLICK_TO_SELECT_COLOR_STRING,
	CLOSE_HELP_STRING,
	CLOSE_STRING,
	COLOR_AMOUNT_STRING,
	COLOR_MODEL_STRING,
	COLOR_SET_STRING,
	COLOR_STRING,
	COLOR_VARIANCE_STRING,
	COLOR_WINDOW_STRING,
	COLORS_STRING,
	CONFIRM_QUIT_STRING,
	CONTINUOUS_STRING,
	COPY_STRING,
	CORNER_TO_CORNER_STRING,
	CREATE_CANVAS_STRING,
	CROP_HELP_STRING,
	CROP_STRING,
	CROSS_HAIR_CURSOR_STRING,
	CURSOR_STRING,
	CUT_STRING,
	DELETE_CURRENT_SET_STRING,
	DELETE_LAYER_STRING,
	DELETE_SELECTED_BRUSH_STRING,
	DISCARD_CHANGES_STRING,
	DO_NOT_SAVE_STRING,
	DUPLICATE_LAYER_STRING,
	EDIT_STRING,
	EFFECTS_WINDOW_STRING,
	ELLIPSE_STRING,
	EMPTY_PAINT_WINDOW_STRING,
	ENABLE_ANTI_ALIASING_STRING,
	ENABLE_GRADIENT_STRING,
	ENABLE_PREVIEW_STRING,
	ENABLE_ROTATION_STRING,
	FADE_STRING,
	FILE_STRING,
	FILL_ELLIPSE_STRING,
	FILL_RECTANGLE_STRING,
	FINISHING_STRING,
	FLIP_HORIZONTAL_ALL_LAYERS_HELP_STRING,
	FLIP_HORIZONTAL_LAYER_HELP_STRING,
	FLIP_HORIZONTAL_STRING,
	FLIP_VERTICAL_ALL_LAYERS_HELP_STRING,
	FLIP_VERTICAL_LAYER_HELP_STRING,
	FLIP_VERTICAL_STRING,
	FLOOD_FILL_STRING,
	FLOW_STRING,
	FONT_STRING,
	FREE_LINE_STRING,
	GLOBAL_SETTINGS_STRING,
	GREEN_STRING,
	GRID_OFF_HELP_STRING,
	GRID_2_BY_2_HELP_STRING,
	GRID_4_BY_4_HELP_STRING,
	GRID_8_BY_8_HELP_STRING,
	GROW_SELECTION_HELP_STRING,
	GROW_SELECTION_STRING,
	HAIRS_STRING,
	HEIGHT_STRING,
	HELP_STRING,
	INSERT_TEXT_HELP_STRING,
	INSERT_TEXT_STRING,
	INTELLIGENT_SCISSORS_STRING,
	INVERT_SELECTION_HELP_STRING,
	INVERT_SELECTION_STRING,
//	KEEP_BRUSH_WINDOW_FRONT_STRING,
//	KEEP_COLOR_WINDOW_FRONT_STRING,
//	KEEP_EFFECTS_WINDOW_FRONT_STRING,
//	KEEP_LAYER_WINDOW_FRONT_STRING,
//	KEEP_TOOL_SELECTION_WINDOW_FRONT_STRING,
//	KEEP_TOOL_SETUP_WINDOW_FRONT_STRING,
	KEEP_IN_FRONT_STRING,
	LANGUAGE_STRING,
	LAYER_COPY_HELP_STRING,
	LAYER_CUT_HELP_STRING,
	LAYER_STRING,
	LAYER_WINDOW_STRING,
	LAYERS_STRING,
	LEFT_STRING,
	LITTLE_STRING,
	LOCK_PROPORTIONS_STRING,
	MAG_STRING,
	MAGIC_WAND_STRING,
	MERGE_WITH_BACK_LAYER_STRING,
	MERGE_WITH_FRONT_LAYER_STRING,
	MISCELLANEOUS_STRING,
	MODE_STRING,
	MUCH_STRING,
	NEW_COLOR_SET_STRING,
	NEW_PROJECT_HELP_STRING,
	NEW_PROJECT_STRING,
	NO_OPTIONS_STRING,
	NONE_STRING,
	OFF_STRING,
	OK_STRING,
	OPAQUE_STRING,
	OPEN_COLOR_SET_STRING,
	OPEN_IMAGE_HELP_STRING,
	OPEN_IMAGE_STRING,
	OPEN_PROJECT_HELP_STRING,
	OPEN_PROJECT_STRING,
	OPEN_STRING,
	PALETTE_WINDOW_NAME_STRING,
	PASTE_AS_NEW_LAYER_HELP_STRING,
	PASTE_AS_NEW_LAYER_STRING,
	PASTE_AS_NEW_PROJECT_HELP_STRING,
	PASTE_AS_NEW_PROJECT_STRING,
	QUIT_HELP_STRING,
	QUIT_STRING,
	RANDOM_STRING,
	RECTANGLE_STRING,
	RECENT_IMAGES_STRING,
	RECENT_PROJECTS_STRING,
	REDO_HELP_STRING,
	REDO_NOT_AVAILABLE_STRING,
	REDO_STRING,
	RED_STRING,
	RELEASE_DATE_STRING,
	RESIZE_TO_FIT_HELP_STRING,
	RESIZE_TO_FIT_STRING,
	RIGHT_STRING,
	ROTATE_ALL_LAYERS_HELP_STRING,
	ROTATE_LAYER_HELP_STRING,
	ROTATE_STRING,
	ROTATE_CW_STRING,
	ROTATE_CCW_STRING,
	ROTATION_STRING,
	ROTATING_STRING,
	SAVE_CHANGES_STRING,
	SAVE_COLOR_SET_AS_STRING,
	SAVE_COLOR_SET_STRING,
	SAVE_FORMAT_STRING,
	SAVE_IMAGE_AS_HELP_STRING,
	SAVE_IMAGE_AS_STRING,
	SAVE_IMAGE_HELP_STRING,
	SAVE_IMAGE_STRING,
	SAVE_PROJECT_AS_HELP_STRING,
	SAVE_PROJECT_AS_STRING,
	SAVE_PROJECT_HELP_STRING,
	SAVE_PROJECT_STRING,
	SAVE_STRING,
	SCALE_ALL_LAYERS_HELP_STRING,
	SCALE_STRING,
	SELECT_CANVAS_SIZE_STRING,
	SELECTED_COLORS_VIEW_MESSAGE1_STRING,
	SET_GRID_STRING,
	SET_ZOOM_LEVEL_STRING,
	SETTINGS_HELP_STRING,
	SETTINGS_STRING,
	SHAPE_STRING,
	SHEAR_STRING,
	SHORTCUTS_HELP_STRING,
	SHORTCUTS_STRING,
	SHOW_BRUSH_WINDOW_HELP_STRING,
	SHOW_BRUSH_WINDOW_STRING,
	SHOW_LAYER_WINDOW_HELP_STRING,
	SHOW_LAYER_WINDOW_STRING,
	SHOW_PALETTE_WINDOW_HELP_STRING,
	SHOW_PALETTE_WINDOW_STRING,
	SHOW_TOOL_SETUP_WINDOW_HELP_STRING,
	SHOW_TOOL_SETUP_WINDOW_STRING,
	SHOW_TOOL_WINDOW_HELP_STRING,
	SHOW_TOOL_WINDOW_STRING,
	SHRINK_SELECTION_HELP_STRING,
	SHRINK_SELECTION_STRING,
	SIZE_STRING,
	SPEED_STRING,
	SPRAY_STRING,
	STANDARD_SIZES_STRING,
	STORE_BRUSH_STRING,
	SUBTRACT_AREA_STRING,
	TEXT_COLOR_STRING,
	TEXT_STRING,
	TOLERANCE_STRING,
	TOOL_CURSOR_STRING,
	TOOL_SELECTION_WINDOW_STRING,
	TOOLS_STRING,
	TOOL_SETUP_STRING,
	TOOL_SETUP_WINDOW_STRING,
	TOP_STRING,
	TRANSLATE_ALL_LAYERS_HELP_STRING,
	TRANSLATE_LAYER_HELP_STRING,
	TRANSLATE_STRING,
	TRANSPARENCY_STRING,
	TRANSPARENT_STRING,
	UNDO_HELP_STRING,
	UNDO_NOT_AVAILABLE_STRING,
	UNDO_SETTINGS_STRING,
	UNDO_STRING,
	UNLIMITED_STRING,
	UNSUPPORTED_FILE_TYPE_STRING,
	UNTITLED_STRING,
	USE_THE_TOOL_STRING,
	USER_MANUAL_HELP_STRING,
	USER_MANUAL_STRING,
	USING_THE_TOOL_STRING,
	VISIBLE_STRING,
	WAND_TOLERANCE_STRING,
	WIDTH_STRING,
	WINDOW_FLOATING_STRING,
	WINDOW_STRING,
	ZOOM_IN_HELP_STRING,
	ZOOM_IN_STRING,
	ZOOM_LEVEL_25_HELP_STRING,
	ZOOM_LEVEL_50_HELP_STRING,
	ZOOM_LEVEL_100_HELP_STRING,
	ZOOM_LEVEL_200_HELP_STRING,
	ZOOM_LEVEL_400_HELP_STRING,
	ZOOM_LEVEL_800_HELP_STRING,
	ZOOM_OUT_HELP_STRING,
	ZOOM_OUT_STRING,

	// Here are the constants for tool-names and tool help-strings.
	AIR_BRUSH_TOOL_NAME_STRING,
	BLUR_TOOL_NAME_STRING,
	BRUSH_TOOL_NAME_STRING,
	COLOR_SELECTOR_TOOL_NAME_STRING,
	ELLIPSE_TOOL_NAME_STRING,
	ERASER_TOOL_NAME_STRING,
	FILL_TOOL_NAME_STRING,
	FREE_LINE_TOOL_NAME_STRING,
	HAIRY_BRUSH_TOOL_NAME_STRING,
	RECTANGLE_TOOL_NAME_STRING,
	SELECTOR_TOOL_NAME_STRING,
	STRAIGHT_LINE_TOOL_NAME_STRING,
	TEXT_TOOL_NAME_STRING,
	TRANSPARENCY_TOOL_NAME_STRING,

	AIR_BRUSH_TOOL_READY_STRING,
	BLUR_TOOL_READY_STRING,
	BRUSH_TOOL_READY_STRING,
	COLOR_SELECTOR_TOOL_READY_STRING,
	ELLIPSE_TOOL_READY_STRING,
	ERASER_TOOL_READY_STRING,
	FILL_TOOL_READY_STRING,
	FREE_LINE_TOOL_READY_STRING,
	HAIRY_BRUSH_TOOL_READY_STRING,
	RECTANGLE_TOOL_READY_STRING,
	SELECTOR_TOOL_READY_STRING,
	STRAIGHT_LINE_TOOL_READY_STRING,
	TEXT_TOOL_READY_STRING,
	TRANSPARENCY_TOOL_READY_STRING,

	AIR_BRUSH_TOOL_IN_USE_STRING,
	BLUR_TOOL_IN_USE_STRING,
	BRUSH_TOOL_IN_USE_STRING,
	COLOR_SELECTOR_TOOL_IN_USE_STRING,
	ELLIPSE_TOOL_IN_USE_STRING,
	ERASER_TOOL_IN_USE_STRING,
	FILL_TOOL_IN_USE_STRING,
	FREE_LINE_TOOL_IN_USE_STRING,
	HAIRY_BRUSH_TOOL_IN_USE_STRING,
	RECTANGLE_TOOL_IN_USE_STRING,
	SELECTOR_TOOL_IN_USE_STRING,
	STRAIGHT_LINE_TOOL_IN_USE_STRING,
	TEXT_TOOL_IN_USE_STRING,
	TRANSPARENCY_TOOL_IN_USE_STRING,



	// Here begin the constants for manipulator help-strings
	DO_CHANGE_TRANSPARENCY_HELP_STRING,
	DO_CROP_HELP_STRING,
	DO_ROTATE_HELP_STRING,
	DO_TRANSLATE_HELP_STRING,
	DO_SCALE_HELP_STRING,


	// Here begin the constants for use in the about-window
	ABOUT_1_TEXT_STRING,
	//ABOUT_2_TEXT_STRING, // Was used for shareware message, now removed
	ABOUT_3_TEXT_STRING,
	ABOUT_4_TEXT_STRING,
	ABOUT_5_TEXT_STRING,
	ABOUT_6_TEXT_STRING,
	ABOUT_7_TEXT_STRING,
	ABOUT_8_TEXT_STRING,
	ABOUT_9_TEXT_STRING,
	ABOUT_10_TEXT_STRING,

	// These are general alert strings
	MEMORY_ALERT_1_STRING,
	MEMORY_ALERT_2_STRING,
	MEMORY_ALERT_3_STRING,
};


class StringServer {
static	languages	current_language;

static	const	char*	return_english_string(string_id);
static	const	char*	return_finnish_string(string_id);
static	const	char*	return_german_string(string_id);
static	const	char*	return_french_string(string_id);

public:
static	const	char*	ReturnString(string_id);
static			void	SetLanguage(languages);
};


#endif
