/*
 * Copyright 2003, Heikki Suhonen
 * Copyright 2008, Karsten Heimrich
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Heikki Suhonen <heikki.suhonen@gmail.com>
 *		Karsten Heimrich <karsten.heimrich@gmx.de>
 *
 */
#ifndef MAGNIFICATION_VIEW_H
#define	MAGNIFICATION_VIEW_H


#include <Box.h>


class BButton;
class MagStringView;


class MagnificationView : public BBox {
public:
								MagnificationView();

	virtual	void				AttachedToWindow();
	virtual	void				Draw(BRect updateRect);

			void				SetTarget(const BMessenger& target);
			void				SetMagnificationLevel(float magLevel);

private:
			BButton*			fPlusButton;
			BButton*			fMinusButton;
			MagStringView*		fMagStringView;
};


#endif
