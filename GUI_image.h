/*
    GUILIB:  An example GUI framework library for use with SDL
*/

/* This is a very simple image widget */

#ifndef _GUI_image_h
#define _GUI_image_h

#include "GUI_widget.h"

class GUI_Image : public GUI_Widget {

public:
	/* Passed the position and BMP image file */
	GUI_Image(int x, int y, char *file);

	/* Passed the position and image surface */
	GUI_Image(int x, int y, SDL_Surface *pic, int shouldfree = 0);

	virtual ~GUI_Image();

	/* Show the widget  */
	virtual void Display(void);

protected:
	/* The display image */
	SDL_Surface *image;

	/* Whether or not we should free the image when we're done */
	int free_image;
};

#endif /* _GUI_image_h */
