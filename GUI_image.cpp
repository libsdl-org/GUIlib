/*
    GUILIB:  An example GUI framework library for use with SDL
*/

/* This is a very simple image widget */

#include "GUI_image.h"

/* Passed the position and BMP image file */
GUI_Image:: GUI_Image(int x, int y, char *file)
 : GUI_Widget(NULL)
{
	SDL_Surface *picture;

	picture = SDL_LoadBMP(file);
	if ( picture == NULL ) {
		SetError("Couldn't load %s: %s", file, SDL_GetError());
		return;
	}
//*****	GUI_Image(x, y, picture, 1); ***ERROR*** another constructor****
//*****	instead: set size manually...
	SetRect(x,y,picture->w,picture->h);
	image=picture;
	free_image=1;
}

/* Passed the position and image */
GUI_Image:: GUI_Image(int x, int y, SDL_Surface *pic, int shouldfree)
 : GUI_Widget(NULL, x, y, pic->w, pic->h)
{
	image = pic;
	free_image = shouldfree;
}

GUI_Image::~GUI_Image()
{
	if ( free_image ) {
		SDL_FreeSurface(image);
	}
}

/* Show the widget  */
void
GUI_Image:: Display(void)
{
	SDL_BlitSurface(image, NULL, screen, &area);
}
