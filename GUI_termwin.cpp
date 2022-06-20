/*
    GUILIB:  An example GUI framework library for use with SDL
*/

/* A simple dumb terminal scrollable widget */

#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "GUI_termwin.h"
#include "GUI_loadimage.h"

#define KEYREPEAT_TIME	100		/* Repeat every 100 ms */

GUI_TermWin:: GUI_TermWin(int x, int y, int w, int h, SDL_Surface *Font,
				void (*KeyProc)(SDLKey key, Uint16 unicode), int scrollback)
 : GUI_Scrollable(NULL, x, y, w, h)
{
	/* The font surface should be a 16x16 character pixmap */
	if ( Font == NULL ) {
		font = GUI_DefaultFont();
	} else {
		font = Font;
	}
	charh = font->h/16;
	charw = font->w/16;
	rows = h/(charh-1);   /* Bottom row in font is not displayed */
	cols = w/charw;
	if ( scrollback == 0 ) {
		scrollback = rows;
	}
	total_rows = scrollback;

	/* Allocate and clear the character buffer */
	vscreen = new Uint8[total_rows*cols];
	Clear();

	/* Set the user-defined keyboard handler */
	keyproc = KeyProc;
	repeat_key = SDLK_UNKNOWN;
	repeat_unicode = 0;

	/* Set key event translation on */
	translated = SDL_EnableUNICODE(1);
}

GUI_TermWin:: ~GUI_TermWin()
{
	delete[] vscreen;

	/* Reset key event translation */
	SDL_EnableUNICODE(translated);
}

void
GUI_TermWin:: Display(void)
{
	int row, i, j;
	Uint8 ch;
	SDL_Rect src;
	SDL_Rect dst;

	row = first_row+scroll_row;
	if ( row < 0 ) {
		row = total_rows+row;
	}
	src.w = charw;
	src.h = charh-1;
	dst.w = charw;
	dst.h = charh-1;
	for ( i=0; i<rows; ++i ) {
		for ( j=0; j<cols; ++j ) {
			ch = vscreen[row*cols+j];
			src.x = (ch%16)*charw;
			src.y = (ch/16)*charh;
			dst.x = area.x+j*charw;
			dst.y = area.y+i*(charh-1);
			SDL_BlitSurface(font, &src, screen, &dst);
		}
		row = (row+1)%total_rows;
	}
	changed = 0;
}

int
GUI_TermWin:: Scroll(int amount)
{
	if ( amount ) {
		scroll_row += amount;
		if ( scroll_row < scroll_min ) {
			scroll_row = scroll_min;
		} else
		if ( scroll_row > scroll_max ) {
			scroll_row = scroll_max;
		}
		changed = 1;
	}
	return(scroll_row);
}

void
GUI_TermWin:: Range(int &first, int &last)
{
	first = scroll_min; last = scroll_max;
}

GUI_status
GUI_TermWin:: KeyDown(SDL_keysym key)
{
	GUI_status status;

	status = GUI_PASS;
	if ( keyproc ) {
		keyproc(key.sym, key.unicode);
		repeat_key = key.sym;
		repeat_unicode = key.unicode;
		repeat_next = SDL_GetTicks()+5*KEYREPEAT_TIME;
		status = GUI_YUM;
	}
	return(status);
}

GUI_status
GUI_TermWin:: KeyUp(SDL_keysym key)
{
	repeat_key = SDLK_UNKNOWN;
	return(GUI_PASS);
}

void
GUI_TermWin:: NewLine(void)
{
	if ( cur_row == (rows-1) ) {
		int newrow;

		first_row = (first_row+1)%total_rows;
		newrow = (first_row+rows-1)%total_rows;
		memset(&vscreen[newrow*cols], ' ', cols);
	} else {
		cur_row += 1;
	}
	cur_col = 0;
}

void
GUI_TermWin:: AddText(const char *text, int len)
{
	int row;

	while ( len-- ) {
		switch (*text) {
			case '\b': {
				/* Backspace */
				if ( cur_col > 0 ) {
					--cur_col;
					row = (first_row+cur_row)%total_rows;
					vscreen[row*cols+cur_col] = ' ';
				}
			}
			break;

			case '\r':
				/* Skip '\n' in "\r\n" sequence */
				if ( (len > 0) && (*(text+1) == '\n') ) {
					--len; ++text;
				}
				/* Fall through to newline */
			case '\n': {
				NewLine();
			}
			break;

			default: {  /* Put characters on screen */
				if ( cur_col == cols ) {
					NewLine();
				}
				row = (first_row+cur_row)%total_rows;
				vscreen[row*cols+cur_col] = *text;
				cur_col += 1;
			}
			break;
		}
		++text;
	}
	/* Reset any scrolling, and mark us for update on idle */
	scroll_row = 0;
	changed = 1;
}

void
GUI_TermWin:: AddText(const char *fmt, ...)
{
	char text[1024];		/* Caution!  Buffer overflow!! */
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	AddText(text, strlen(text));
	va_end(ap);
}

void
GUI_TermWin:: Clear(void)
{
	/* Clear the virtual screen */
	memset(vscreen, ' ', total_rows*cols);

	/* Set the first row in buffer, display row in buffer, and
	   current cursor offset.
	 */
	first_row = total_rows-rows;		/* Actual address */
	scroll_min = -(total_rows-rows);	/* Virtual address */
	scroll_row = 0;				/* Virtual address */
	scroll_max = 0;				/* Virtual address */
	cur_row = 0;				/* Virtual address */
	cur_col = 0;				/* Virtual address */

	/* Mark the display for update */
	changed = 1;
}

int
GUI_TermWin:: Changed(void)
{
	return(changed);
}

GUI_status
GUI_TermWin:: Idle(void)
{
	GUI_status status;

	status = GUI_PASS;

	/* Perform any necessary key repeat */
	if ( repeat_key && keyproc ) {
		if ( repeat_next <= SDL_GetTicks() ) {
			keyproc(repeat_key, repeat_unicode);
			repeat_next = SDL_GetTicks()+KEYREPEAT_TIME;
		}
	}

	/* Check to see if display contents have changed */
	if ( changed ) {
		status = GUI_REDRAW;
		changed = 0;
	}
	return(status);
}

/* change FG/BG colors and transparency */
void GUI_TermWin::SetColoring(Uint8 fr,Uint8 fg,Uint8 fb, int bg_opaque,
				Uint8 br, Uint8 bg, Uint8 bb)
{
	SDL_Color colors[3]={{br,bg,bb,0},{fr,fg,fb,0}};
	if (bg_opaque)
	{
	  SDL_SetColors(font,colors,0,2);
	  SDL_SetColorKey(font,0,0);
	}
	else
	{
	  SDL_SetColors(font,&colors[1],1,1);
	  SDL_SetColorKey(font,SDL_SRCCOLORKEY,0);
	}
}
