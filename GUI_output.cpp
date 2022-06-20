
/* This is a set of C functions wrapping C++ classes for popup text output */

#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "GUI.h"
#include "GUI_widgets.h"
#include "GUI_loadimage.h"
#include "GUI_output.h"

/************************************************************************/
/* C functions for GUI output window support                            */
/*                                                                      */
/************************************************************************/

static GUI_status OK_ButtonProc(void *statusp)
{
	*((int *)statusp) = 1;
	return(GUI_QUIT);
}
static GUI_status Cancel_ButtonProc(void *statusp)
{
	*((int *)statusp) = 0;
	return(GUI_QUIT);
}

extern "C" {

/* The (really C++) structure holding information for popup text output */
struct GUI_Output {
	int visible;
	SDL_Surface *screen;
	GUI_TermWin *window;
	GUI_Area *frame_inner;
	GUI_Area *frame_outer;
	SDL_Surface *behind;
};

/* Function to create a hidden output window 'width' by 'height' characters.
   The 'font' argument should either be a pointer to a 16x16 character font
   image (with an extra pixel row under each row of characters), or NULL to
   use a default internal 8x8-pixel font.
 */
GUI_Output *GUI_CreateOutput(SDL_Surface *screen,
				int width, int height, SDL_Surface *font)
{
	GUI_Output *output;
	int w, h, x, y;

	/* Create a new output window */
	output = new GUI_Output;
	output->visible = 0;
	output->screen = screen;
	if ( font == NULL ) {
		font = GUI_DefaultFont();
	}

	/* Allocate the text window */
	w = width * (font->w/16);
	h = height * ((font->h/16)-1);
	x = (screen->w - w)/2;
	y = (screen->h - h)/2;
	output->window = new GUI_TermWin(x, y, w, h, font);

	/* Allocate the frame areas */
	w += 2; h += 2;
	x -= 1; y -= 1;
	output->frame_inner = new GUI_Area(x, y, w, h, 255, 255, 255);
	w += 2; h += 2;
	x -= 1; y -= 1;
	output->frame_outer = new GUI_Area(x, y, w, h, 0, 0, 0);

	/* Allocate a save buffer for the area behind it */
	output->behind = SDL_AllocSurface(SDL_SWSURFACE, w, h,
				screen->format->BitsPerPixel,
				screen->format->Rmask,
				screen->format->Gmask,
				screen->format->Bmask, 0);

	/* Return the output window */
	return(output);
}

/* Add output to an output window.  If the window is visible, the output
   will appear immediately.  Note that the output windows are not overlays,
   and any normal SDL drawing will overwrite the output window display.
   If output causes the window to scroll, previous text will be lost.
*/
void GUI_AddOutput(GUI_Output *output, const char *fmt, ...)
{
	va_list ap;
	char text[4096];

	/* Get the text string */
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);	/* CAUTION: possible buffer overflow */
	va_end(ap);

	/* Add it to the window */
	output->window->AddText(text, strlen(text));
}

/* Clear the contents of an output window */
void GUI_ClearOutput(GUI_Output *output)
{
	output->window->Clear();
}

/* Show an output window, saving the area behind the window, and wait for
   keyboard or mouse press input if 'wait' is non-zero.
 */
void GUI_ShowOutput(GUI_Output *output, int wait)
{
	/* Display the text output window */
	output->frame_outer->SetDisplay(output->screen);
	output->frame_inner->SetDisplay(output->screen);
	output->window->SetDisplay(output->screen);
	if ( output->behind ) {
		SDL_Rect src;

		src.x = output->frame_outer->X();
		src.y = output->frame_outer->Y();
		src.w = output->frame_outer->W();
		src.h = output->frame_outer->H();
		SDL_BlitSurface(output->screen, &src, output->behind, NULL);
	}
	output->frame_outer->Display();
	output->frame_inner->Display();
	output->window->Display();
	SDL_UpdateRect(output->screen, 0, 0, 0, 0);
	output->visible = 1;

	/* Pump the event queue, waiting for key and mouse press events */
	if ( wait ) {
		SDL_Event event;

		while ( ! SDL_PeepEvents(&event, 1, SDL_GETEVENT,
				(SDL_KEYDOWNMASK|SDL_MOUSEBUTTONDOWNMASK)) ) {
			SDL_Delay(20);
			SDL_PumpEvents();
		}
	}
}

/* Hide an output window, restoring the previous contents of the display */
void GUI_HideOutput(GUI_Output *output)
{
	if ( output->behind ) {
		SDL_Rect dst;

		dst.x = output->frame_outer->X();
		dst.y = output->frame_outer->Y();
		dst.w = output->frame_outer->W();
		dst.h = output->frame_outer->H();
		SDL_BlitSurface(output->behind, NULL, output->screen, &dst);
		SDL_UpdateRects(output->screen, 1, &dst);
	}
	output->visible = 0;
}

/* Delete an output window */
void GUI_DeleteOutput(GUI_Output *output)
{
	if ( output ) {
		if ( output->visible ) {
			GUI_HideOutput(output);
		}
		if ( output->window ) {
			delete output->window;
			output->window = NULL;
		}
		if ( output->behind ) {
			SDL_FreeSurface(output->behind);
			output->behind = NULL;
		}
		delete output;
	}
}


/************************************************************************/
/* Simple C message-box functions for quick popup output                */
/*                                                                      */
/************************************************************************/

#include "okay_img.h"
#include "cancel_img.h"


/* Create a popup message box of the given style, and wait for user input.
   If 'title' is NULL, then no title-text will be used.

   Returns the index of the button pressed, which is style dependent:
*/
int GUI_MessageBox(SDL_Surface *screen,
			const char *title, const char *text, Uint32 style)
{
	int status;
	const int gui_w = 320, gui_h = 200;
	GUI *gui;
	GUI_Widget *widget;
	GUI_TermWin *textout;
	GUI_Button *button;
	int w, h, x, y;
	unsigned int i;
	SDL_Surface *font;
	SDL_Surface *images[2];
	SDL_Surface *background;
	SDL_Rect srcrect;
	SDL_Rect dstrect;

	/* Initialize status */
	status = -1;

	/* Create the GUI holder */
	gui = new GUI(screen);
	font = GUI_DefaultFont();

	/* Create the GUI elements */
	w = gui_w;
	h = gui_h;
	x = (screen->w - w)/2;
	y = (screen->h - h)/2;

	/* Save the message box background to a private surface */
	srcrect.x = x;
	srcrect.y = y;
	srcrect.w = w;
	srcrect.h = h;
	dstrect = srcrect;
	background = SDL_AllocSurface(SDL_SWSURFACE, w, h, 
				screen->format->BitsPerPixel,
				screen->format->Rmask,
				screen->format->Gmask,
				screen->format->Bmask, 0);
	SDL_BlitSurface(screen, &srcrect, background, NULL);

	/* Adjust for the height of buttons */
	if ( style != GUI_MBNONE ) {
		h -= 1+32+1;
	}

	/* Create the title, if any */
	if ( title ) {
		char titletext[1024];
		int title_x, title_y;
		int title_w, title_h;

		/* Set up the title coordinates */
		title_x = x;
		title_y = x;
		title_w = w;
		title_h = ((font->h/16)-1) + 4;
		y += title_h;
		h -= title_h;

		/* Add the outer frame */
		widget = new GUI_Area(title_x, title_y, title_w, title_h,
							0, 0, 0);
		gui->AddWidget(widget);

		/* Add the inner frame */
		title_x += 1; title_y += 1;
		title_h -= 1; title_w -= 2;
		widget = new GUI_Area(title_x, title_y, title_w, title_h,
							255, 255, 255);
		gui->AddWidget(widget);
		
		/* Add the centered title text */
		title_x += 1; title_y += 1;
		title_h -= 2; title_w -= 2;
		textout = new GUI_TermWin(title_x, title_y, title_w, title_h,
							font);
		for ( i=0; i<((title_w/(font->w/16))-strlen(title))/2; ++i ) {
			titletext[i] = ' ';
		}
		titletext[i] = '\0';
		strcat(titletext, title);
		textout->AddText(titletext, strlen(titletext));
		gui->AddWidget(textout);
	}

	/* Create outer frame */
	widget = new GUI_Area(x, y, w, h, 0, 0, 0);
	gui->AddWidget(widget);

	/* Create inner frame */
	x += 1; y += 1;
	w -= 2; h -= 2;
	widget = new GUI_Area(x, y, w, h, 255, 255, 255);
	gui->AddWidget(widget);

	/* Create text output area */
	x += 1; y += 1;
	h -= 2; w -= 2;
	textout = new GUI_TermWin(x, y, w, h, font);
	textout->AddText(text, strlen(text));
	gui->AddWidget(textout);

	/* Create any buttons */
	y += h+2;
	for ( i=0; i<2; ++i ) {
		images[i] = NULL;
	}
	if ( style == GUI_MBNONE ) {
		y += 1;
	} else {
		/* Create button frame */
		x = (screen->w - gui_w)/2;
		w = gui_w;
		h = 1+32+1;
		widget = new GUI_Area(x, y, w, h, 0, 0, 0);
		gui->AddWidget(widget);
		x += 1; y += 1;
		w -= 2; h -= 2;
		widget = new GUI_Area(x, y, w, h, 0xAA, 0xAA, 0xAA);
		gui->AddWidget(widget);
	}
	switch (style) {
		case GUI_MBNONE:	/* Create a single large button */
			x = (screen->w - gui_w)/2;
			y = (screen->h - gui_h)/2;
			button = new GUI_Button(NULL, x, y, gui_w, gui_h);
			gui->AddWidget(button);
			break;
		case GUI_MBOK:	/* Create a single "OK" button */
			images[0] = GUI_LoadImage(okay_w, okay_h, okay_pal,
								okay_data);
			x = (screen->w - images[0]->w)/2;
			button = new GUI_Button(&status, x, y,images[0],NULL,
							OK_ButtonProc);
			gui->AddWidget(button);
			break;
		case GUI_MBOKCANCEL: /* Create "OK" and "Cancel" buttons */
			images[0] = GUI_LoadImage(cancel_w, cancel_h,
						cancel_pal, cancel_data);
			x = (screen->w - gui_w)/2 + 1;
			button = new GUI_Button(&status, x, y,images[0],NULL,
							Cancel_ButtonProc);
			gui->AddWidget(button);
			images[1] = GUI_LoadImage(okay_w, okay_h, okay_pal,
								okay_data);
			x = screen->w - ((screen->w - gui_w)/2 + 1) -
							images[0]->w;
			button = new GUI_Button(&status, x, y,images[1],NULL,
							OK_ButtonProc);
			gui->AddWidget(button);
			break;
	}

	/* Run the GUI */
	gui->Run();

	/* Clean up, restore background, and return the status */
	delete gui;
	for ( i=0; i<2; ++i ) {
		if ( images[i] ) {
			SDL_FreeSurface(images[i]);
		}
	}
	SDL_BlitSurface(background, NULL, screen, &dstrect);
	SDL_UpdateRects(screen, 1, &dstrect);
	return(status);
}

}; /* Extern C */
