/*
    GUILIB:  An example GUI framework library for use with SDL
*/

/* This is a simple colored area widget -- rectangular or round  */

#ifndef _GUI_area_h
#define _GUI_area_h

#include "GUI_widget.h"

#define AREA_ROUND 1
#define AREA_ANGULAR 2

class GUI_Area : public GUI_Widget {

public:
	/* Passed the area, color and shape */
	GUI_Area(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, int aShape = AREA_ANGULAR);

	/* Passed the area, color, frame color, frame thickness and shape */
	GUI_Area(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b,
			Uint8 fr, Uint8 fg, Uint8 fb, int fthick, int aShape = AREA_ANGULAR);

	/* Map the color to the display */
	virtual void SetDisplay(SDL_Surface *display);

	/* Show the widget  */
	virtual void Display(void);

protected:
	Uint8 R, G, B;
	Uint32 color;

	/* flag */
	int useFrame;

	/* frame color values */
	Uint8 fR,fG,fB;
	Uint32 frameColor;

	/* remember me */
	int frameThickness;
	int shape;
};

#endif /* _GUI_area_h */
