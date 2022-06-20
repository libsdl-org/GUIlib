/*
    GUILIB:  An example GUI framework library for use with SDL
*/

/* A simple dumb terminal scrollable widget */

#ifndef _GUI_termwin_h
#define _GUI_termwin_h

#include "GUI_widget.h"
#include "GUI_scroll.h"


class GUI_TermWin : public GUI_Scrollable {

public:
	/* The font surface should be a 16x16 character pixmap */
	GUI_TermWin(int x, int y, int w, int h, SDL_Surface *font = NULL,
			void (*KeyProc)(SDLKey key, Uint16 unicode) = NULL, int scrollback = 0);
	~GUI_TermWin();

	/* Show the text window */
	virtual void Display(void);

	/* Function to scroll forward and backward */
	virtual int Scroll(int amount);

	/* Function to get the scrollable range */
	virtual void Range(int &first, int &last);

	/* Handle keyboard input */
	virtual GUI_status KeyDown(SDL_keysym key);
	virtual GUI_status KeyUp(SDL_keysym key);

	/* Function to add text to the visible buffer */
	virtual void AddText(const char *text, int len);
	virtual void AddText(const char *fmt, ...);

	/* Function to clear the visible buffer */
	virtual void Clear(void);

	/* Function returns true if the window has changed */
	virtual int Changed(void);

	/* GUI idle function -- run when no events pending */
	virtual GUI_status Idle(void);

	/* change FG/BG colors and transparency */
	virtual void SetColoring(Uint8 fr, Uint8 fg, Uint8 fb, int bg_opaque = 0,
					 Uint8 br = 0, Uint8 bg = 0, Uint8 bb = 0);

protected:
	/* The actual characters representing the screen */
	Uint8 *vscreen;

	/* Information about the size and position of the buffer */
	int total_rows;
	int rows, cols;
	int first_row;
	int cur_row;
	int cur_col;
	int scroll_min, scroll_row, scroll_max;

	/* Font information used to display characters */
	SDL_Surface *font;
	int charw, charh;
	int translated;	/* Whether or not UNICODE translation was enabled */

	/* The keyboard handling function */
	void (*keyproc)(SDLKey key, Uint16 unicode);

	/* The last key that was pressed, along with key repeat time */
	SDLKey repeat_key;
	Uint16 repeat_unicode;
	Uint32 repeat_next;

	/* Flag -- whether or not we need to redisplay ourselves */
	int changed;

	/* Terminal manipulation routines */
	void NewLine(void);			/* <CR>+<LF> */
};

#endif /* _GUI_termwin_h */
