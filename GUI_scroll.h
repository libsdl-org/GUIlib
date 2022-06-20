/*
    GUILIB:  An example GUI framework library for use with SDL
*/

/* This is a virtual base class for a scrollable widget */

#ifndef _GUI_scroll_h
#define _GUI_scroll_h

#include "GUI_widget.h"


class GUI_Scrollable : public GUI_Widget {

public:
	/* Passed the generic widget parameters */
	GUI_Scrollable(void *data)	: GUI_Widget(data) {
	}
	GUI_Scrollable(void *data, int x, int y, int w, int h)
					: GUI_Widget(data, x, y, w, h) {
	}

	/* Virtual functions to scroll forward and backward
	   This function returns the final position of the cursor.
	 */
	virtual int Scroll(int amount) = 0;

	/* Virtual function to get the scrollable range */
	virtual void Range(int &first, int &last) = 0;
};

#endif /* _GUI_scroll_h */
