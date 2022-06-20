/*
    GUILIB:  An example GUI framework library for use with SDL
*/

/* This is a very generic scrollbar widget */

#ifndef _GUI_scrollbar_h
#define _GUI_scrollbar_h

#include "GUI_widget.h"
#include "GUI_scroll.h"

/* The possible orientation of the scrollbar */
typedef enum { SCROLLBAR_HORIZONTAL, SCROLLBAR_VERTICAL } orientation;

class GUI_ScrollBar : public GUI_Widget {

public:
	/* Passed the sensitive areas, orientation, and initial widget */
	GUI_ScrollBar(SDL_Rect &first, SDL_Rect &middle, SDL_Rect &last,
			orientation direction, GUI_Scrollable *widget = NULL);
	GUI_ScrollBar(orientation direction, GUI_Scrollable *widget = NULL);

	/* Link a scrollable widget to this scrollbar */
	virtual int AddScrollable(GUI_Scrollable *widget);

	/* Show the widget  */
	virtual void Display(void) = 0;

	/* Continue scrolling while mouse is held down */
	virtual GUI_status Idle(void);

	/* Mouse hits activate us */
	virtual GUI_status MouseDown(int x, int y, int button);

protected:
	/* The scrollable widget */
	GUI_Scrollable *target;

	/* The areas:  Scroll up 1,  Position at, Scroll down 1 */
	SDL_Rect sensitive_neg;
	SDL_Rect sensitive_mid;
	SDL_Rect sensitive_pos;
	orientation whichxy;

	/* Time at which the display will be scrolled while the mouse button
	   is being held down.
	 */
	Uint32 next_scroll;

	/* Function to find the bounds of the widget */
	virtual void FindBounds(void);

	/* The functions to activate the scrolling */
	virtual void Scroll(int amount);
	virtual void ScrollTo(int position);
};


class GUI_ScrollButtons : public GUI_ScrollBar {

public:
	/* The possible orientation of the scrollbar:
		GUI_ScrollBar::HORIZONTAL, GUI_ScrollBar::VERTICAL
	 */

	/* Passed the sensitive images, orientation, and initial widget */
	GUI_ScrollButtons(int x1, int y1, SDL_Surface *image1, SDL_Rect &middle,
	                  int x2, int y2, SDL_Surface *image2,
			orientation direction, GUI_Scrollable *widget = NULL);

	/* Show the widget  */
	virtual void Display(void);

protected:
	/* scroll button images */
	SDL_Surface *image_neg;
	SDL_Surface *image_pos;
};

#endif /* _GUI_scrollbar_h */
