/*
    GUILIB:  An example GUI framework library for use with SDL
*/

#include "GUI_widget.h"


/* Widget constructors */
GUI_Widget:: GUI_Widget(void *data)
{
	Init(data, 0, 0, 0, 0);
}
GUI_Widget:: GUI_Widget(void *data, int x, int y, int w, int h)
{
	Init(data, x, y, w, h);
}
void
GUI_Widget:: Init(void *data, int x, int y, int w, int h)
{
	widget_data = data;
	screen = NULL;
	SetRect(x, y, w, h);
	Show();
	error = NULL;
	for (int n=0;n<3; ++n ) {
		pressed[n]=0;
	}
}

/* Mark the widget as visible -- this is the default state */
void
GUI_Widget:: Show(void)
{
	status = WIDGET_VISIBLE;
}

/* Mark the widget as hidden;  no display, no events */
void
GUI_Widget:: Hide(void)
{
	status = WIDGET_HIDDEN;
}

/* Mark the widget as free, so it will be deleted by the GUI */
void
GUI_Widget:: Delete(void)
{
	status = WIDGET_DELETED;
}

/* Report status to GUI */
int
GUI_Widget:: Status(void)
{
	return(status);
}

/* Set the bounds of the widget.
   If 'w' or 'h' is -1, that parameter will not be changed.
 */
void 
GUI_Widget:: SetRect(int x, int y, int w, int h)
{
	area.x = x;
	area.y = y;
	if ( w >= 0 ) {
		area.w = w;
	}
	if ( h >= 0 ) {
		area.h = h;
	}
}
void 
GUI_Widget:: SetRect(SDL_Rect **bounds)
{
	int minx, maxx;
	int miny, maxy;
	int i, v;

	maxx = 0;
	maxy = 0;
	for ( i=0; bounds[i]; ++i ) {
		v = (bounds[i]->x+bounds[i]->w-1);
		if ( maxx < v ) {
			maxx = v;
		}
		v = (bounds[i]->y+bounds[i]->h-1);
		if ( maxy < v ) {
			maxy = v;
		}
	}
	minx = maxx;
	miny = maxy;
	for ( i=0; bounds[i]; ++i ) {
		v = bounds[i]->x;
		if ( minx > v ) {
			minx = v;
		}
		v = bounds[i]->y;
		if ( miny > v ) {
			miny = v;
		}
	}
	SetRect(minx, miny, (maxx-minx+1), (maxy-miny+1));
}

/* Check to see if a point intersects the bounds of the widget.
 */
int
GUI_Widget:: HitRect(int x, int y)
{
	return(HitRect(x, y, area));
}
int
GUI_Widget:: HitRect(int x, int y, SDL_Rect &rect)
{
	int hit;

	hit = 1;
	if ( (x < rect.x) || (x >= (rect.x+rect.w)) ||
	     (y < rect.y) || (y >= (rect.y+rect.h)) ) {
		hit = 0;
	}
	return(hit);
}

/* Set the display surface for this widget */
void
GUI_Widget:: SetDisplay(SDL_Surface *display)
{
	screen = display;
}

/* Show the widget.
   If the surface needs to be locked, it will be locked
   before this call, and unlocked after it returns.

****************NO, NOT AT ALL IF I'M NOT TOO DUMB TO LOOK******
******OTHERWISE YOU COULDN'T FILLRECT in Display(), ETC!!!! ***********
 */
void
GUI_Widget:: Display(void)
{
}

/* Redraw the widget and only the widget */
void GUI_Widget::Redraw(void)
{
  if (status==WIDGET_VISIBLE)
  {
    Display();
    SDL_UpdateRects(screen,1,&area);
  }
}

/* GUI idle function -- run when no events pending */
GUI_status
GUI_Widget:: Idle(void)
{
	return(GUI_PASS);
}

/* Widget event handlers.
   These functions should return a status telling the GUI whether
   or not the event should be passed on to other widgets.
   These are called by the default HandleEvent function.
*/
GUI_status
GUI_Widget:: KeyDown(SDL_keysym key)
{
	return(GUI_PASS);
}
GUI_status
GUI_Widget:: KeyUp(SDL_keysym key)
{
	return(GUI_PASS);
}
GUI_status
GUI_Widget:: MouseDown(int x, int y, int button)
{
	return(GUI_PASS);
}
GUI_status
GUI_Widget:: MouseUp(int x, int y, int button)
{
	return(GUI_PASS);
}
GUI_status
GUI_Widget:: MouseMotion(int x, int y, Uint8 state)
{
	return(GUI_PASS);
}

/* Main event handler function.
   This function gets raw SDL events from the GUI.
 */
GUI_status
GUI_Widget:: HandleEvent(const SDL_Event *event)
{
	switch (event->type) {
		case SDL_KEYDOWN: {
			return(KeyDown(event->key.keysym));
		}
		break;
		case SDL_KEYUP: {
			return(KeyUp(event->key.keysym));
		}
		break;
		case SDL_MOUSEBUTTONDOWN: {
			int x, y, button;
			x = event->button.x;
			y = event->button.y;
			if ( HitRect(x, y) ) {
				button = event->button.button;
				return(MouseDown(x, y, button));
			}
		}
		break;
		case SDL_MOUSEBUTTONUP: {
			int x, y, button;
			x = event->button.x;
			y = event->button.y;
			button = event->button.button;
			if ( HitRect(x, y) )
			{
				button = event->button.button;
				return(MouseUp(x, y, button));
			}
			/* if widget was clicked before we must let it deactivate itself*/
			else if (ClickState(1)) return(MouseUp(-1,-1,button));
		}
		break;
		case SDL_MOUSEMOTION: {
			int x, y;
			Uint8 state;
			x = event->motion.x;
			y = event->motion.y;
			state = event->motion.state;
			if ( HitRect(x, y) )
			{
  			  return(MouseMotion(x, y, state));
			}
			/* if widget was clicked before we must let it react*/
			else if (ClickState(1)) return(MouseMotion(-1,-1,state));
		}
		break;
		default: {
			/* Pass it along.. */;
		}
		break;
	}
	return(GUI_PASS);
}
