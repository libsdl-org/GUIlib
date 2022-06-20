/*
    GUILIB:  An example GUI framework library for use with SDL
*/

#include "GUI_area.h"
#include "math.h"
//#include "GUI_utils.h"

GUI_Area:: GUI_Area(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, int aShape)
 : GUI_Widget(NULL, x, y, w, h)
{
	R = r;
	G = g;
	B = b;
	color = 0;
	useFrame=0;
	shape=aShape;
}

GUI_Area:: GUI_Area(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b,
			Uint8 fr, Uint8 fg, Uint8 fb, int fthick, int aShape)
 : GUI_Widget(NULL, x, y, w, h)
{
	R = r;
	G = g;
	B = b;
	color = 0;
	useFrame=1;
	fR = fr;
	fG = fg;
	fB = fb;
	frameColor=0;
	frameThickness=fthick;
	shape=aShape;
}

/* Map the color to the display */
void
GUI_Area:: SetDisplay(SDL_Surface *display)
{
	GUI_Widget::SetDisplay(display);
	color = SDL_MapRGB(screen->format, R, G, B);
	if (useFrame)
	  frameColor = SDL_MapRGB(screen->format, fR, fG, fB);
}

/* Show the widget  */
void
GUI_Area:: Display(void)
{
	SDL_Rect framerect;
	int x,dy,r1,r2,x0,y0;

	switch (shape)
	{
	case AREA_ROUND:

	  r1=area.w >> 1;
	  r2=area.h >> 1;
	  x0=area.x+r1;
	  y0=area.y+r2;
	  for (x=area.x;x<area.x+area.w;x++)
	  {
	    dy=(int)((double) r2*sin(acos((double) (x-x0)/(double) r1)));
	    framerect.x=x; framerect.y=y0-dy;
	    framerect.w=1; framerect.h=dy << 1;
	    SDL_FillRect(screen,&framerect,color);
	    if (useFrame)
	    {
	      if ((x==area.x) || (x==area.x+area.w-1))
	      {
	        SDL_FillRect(screen,&framerect,frameColor);
	      }
	      framerect.h=frameThickness;
	      SDL_FillRect(screen,&framerect,frameColor);
	      framerect.y=y0+dy-frameThickness;
	      SDL_FillRect(screen,&framerect,frameColor);
	    }
	  }
/*
******** GUI_FillEllipse is not ready yet, GUI_BoundaryFill either *****
	    framerect=area;
	    if (useFrame)
	    {
	      GUI_FillEllipse(screen,&framerect,frameColor);
	      area.x+=frameThickness; area.w-=frameThickness << 1;
	      area.y+=frameThickness; area.h-=frameThickness << 1;
	    }
	    GUI_FillEllipse(screen,&framerect,color);
*/
	  break;

	case AREA_ANGULAR:
	  SDL_FillRect(screen, &area, color);

	  /* draw frame */
	  if (useFrame)
	  {
	    framerect=area;
	    framerect.h=frameThickness;
	    SDL_FillRect(screen, &framerect, frameColor);
	    framerect.y+=area.h-frameThickness;
	    SDL_FillRect(screen, &framerect, frameColor);
	    framerect.y=area.y;
	    framerect.h=area.h;
	    framerect.w=frameThickness;
	    SDL_FillRect(screen, &framerect, frameColor);
	    framerect.x+=area.w-frameThickness;
	    SDL_FillRect(screen, &framerect, frameColor);
	  }
	  break;
	}
}
