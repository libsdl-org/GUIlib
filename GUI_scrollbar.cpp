/*
    GUILIB:  An example GUI framework library for use with SDL
*/

/* This is a very generic scrollbar widget */

#include "GUI_scrollbar.h"


#define SCROLL_INTERVAL	100	/* Scroll every 100 ms */
#define SCROLL_WAIT	2	/* Intervals to wait before scrolling */

/* Passed the sensitive areas, orientation, and initial widget */
GUI_ScrollBar:: GUI_ScrollBar(SDL_Rect &first, SDL_Rect &middle, SDL_Rect &last,
			orientation direction, GUI_Scrollable *widget)
 : GUI_Widget(NULL)
{
	/* Set the sensitive areas */
	sensitive_neg = first;
	sensitive_mid = middle;
	sensitive_pos = last;

	/* Set the orientation and next scrolling time */
	whichxy = direction;
	next_scroll = 0;

	/* Set the default scrollable widget */
	target = widget;
}

GUI_ScrollBar:: GUI_ScrollBar(orientation direction, GUI_Scrollable *widget)
 : GUI_Widget(NULL)
{
	/* Set the orientation */
	whichxy = direction;

	/* Set the default scrollable widget */
	target = widget;
}

void
GUI_ScrollBar:: FindBounds(void)
{
	SDL_Rect *rects[4];

	/* Find the bounds of this widget */
	rects[0] = &sensitive_neg;
	rects[1] = &sensitive_mid;
	rects[2] = &sensitive_pos;
	rects[3] = NULL;
	SetRect(rects);
}

/* Link a scrollable widget to this scrollbar */
int
GUI_ScrollBar:: AddScrollable(GUI_Scrollable *widget)
{
	target = widget;
	return(0);
}

/* Continue scrolling while mouse is held down */
GUI_status
GUI_ScrollBar:: Idle(void)
{
	GUI_status status;

	status = GUI_PASS;
	if ( next_scroll && (next_scroll <= SDL_GetTicks()) ) {
		int x, y;
		Uint8 state;

		state = SDL_GetMouseState(&x, &y);
		if ( (state & SDL_BUTTON(1)) == SDL_BUTTON(1) ) {
			status = MouseDown(x, y, 1);
			next_scroll /= SCROLL_WAIT;
		} else {
			next_scroll = 0;
		}
	}
	return(status);
}

/* Mouse hits activate us */
GUI_status 
GUI_ScrollBar:: MouseDown(int x, int y, int button)
{
	GUI_status status;

	/* Don't do anything if we're not linked to a widget */
	if ( target == NULL ) {
		return(GUI_PASS);
	}

	/* Scroll depending on where we were hit */
	status = GUI_REDRAW;
	if ( HitRect(x, y, sensitive_neg) ) {
		Scroll(-1);
	} else
	if ( HitRect(x, y, sensitive_pos) ) {
		Scroll(1);
	} else
	if ( HitRect(x, y, sensitive_mid) ) {
		int position;
		int first, last;
		float percentage;

		/* Scale the position by the hit coordinate */
		if ( whichxy == SCROLLBAR_HORIZONTAL ) {
			percentage = (float)(x-sensitive_mid.x)/sensitive_mid.w;
		} else {
			percentage = (float)(y-sensitive_mid.y)/sensitive_mid.h;
		}
		target->Range(first, last);
		position = (int)(first+percentage*(last-first)+0.5);
		ScrollTo(position);
	} else {
		status = GUI_PASS;
	}
	if ( status == GUI_REDRAW ) {
		next_scroll = SDL_GetTicks()+SCROLL_WAIT*SCROLL_INTERVAL;
	} else {
		next_scroll = 0;
	}
	return(status);
}

/* The functions to activate the scrolling */
void
GUI_ScrollBar:: Scroll(int amount)
{
	target->Scroll(amount);
}
void
GUI_ScrollBar:: ScrollTo(int position)
{
	target->Scroll(position-target->Scroll(0));
}


/* Passed the sensitive images, orientation, and initial widget */
GUI_ScrollButtons:: GUI_ScrollButtons(int x1, int y1, SDL_Surface *image1,
			SDL_Rect &middle, int x2, int y2, SDL_Surface *image2,
                        	orientation direction, GUI_Scrollable *widget)
 : GUI_ScrollBar(direction, widget)
{
	/* Set the image and rectangle of the negative button */
	image_neg = image1;
	sensitive_neg.x = x1;
	sensitive_neg.y = y1;
	sensitive_neg.w = image1->w;
	sensitive_neg.h = image1->h;

	/* Set the image and rectangle of the positive button */
	image_pos = image2;
	sensitive_pos.x = x2;
	sensitive_pos.y = y2;
	sensitive_pos.w = image2->w;
	sensitive_pos.h = image2->h;

	/* Set the middle sensitive rectangle */
	sensitive_mid = middle;

	/* Find the bounds of this widget */
	FindBounds();
}

/* Show the widget  */
void
GUI_ScrollButtons:: Display(void)
{
	SDL_BlitSurface(image_neg, NULL, screen, &sensitive_neg);
	SDL_BlitSurface(image_pos, NULL, screen, &sensitive_pos);
}
