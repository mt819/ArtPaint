/*
 * Copyright 2003, Heikki Suhonen
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Heikki Suhonen <heikki.suhonen@gmail.com>
 *
 */
#ifndef OIL_ADD_ON_H
#define OIL_ADD_ON_H

#include "Manipulator.h"


class OilManipulator : public Manipulator {
Selection*	selection;
public:
			OilManipulator(BBitmap*);
			~OilManipulator();

BBitmap*	ManipulateBitmap(BBitmap*, BStatusBar*);
const char*	ReturnHelpString();
const char*	ReturnName();
void		SetSelection(Selection* new_selection)
				{ selection = new_selection; };
};

#endif



