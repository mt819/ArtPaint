/*
 * Copyright 2003, Heikki Suhonen
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Heikki Suhonen <heikki.suhonen@gmail.com>
 *
 */
#ifndef DETECT_EDGES_H
#define DETECT_EDGES_H

#include "Manipulator.h"

class DetectEdgesManipulator : public Manipulator {
		BBitmap		*source_bitmap;
		BBitmap		*target_bitmap;
		BStatusBar	*progress_bar;
		Selection	*selection;

static	int32		thread_entry(void*);
		int32		thread_function(int32);
		int			processor_count;

public:
			DetectEdgesManipulator();
			~DetectEdgesManipulator();

BBitmap*	ManipulateBitmap(BBitmap*, BStatusBar*);
const char*	ReturnHelpString();
const char*	ReturnName();
void		SetSelection(Selection* new_selection)
				{ selection = new_selection; }
};
#endif
