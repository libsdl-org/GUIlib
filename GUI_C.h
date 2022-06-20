/*
    GUILIB:  An example GUI framework library for use with SDL
*/

/* This file provides a simple C interface to the C++ GUI classes */

#ifndef _GUI_C_h
#define _GUI_C_h

#include "SDL.h"
#include "GUI_status.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Definitions for the C versions of the GUI and widget objects */
struct CGUI;
typedef struct CGUI CGUI;

struct CGUI_Widget;
typedef struct CGUI_Widget CGUI_Widget;

typedef struct widget_info {
	/* A generic pointer to user-specified data for the widget.
	 */
	void *widget_data;

	/* The display surface for the widget */
	SDL_Surface *screen;

	/* The area covered by the widget */
	SDL_Rect area;

} widget_info;

/* Generic widget callback functions (used by C interface) */
typedef void (*GUI_DrawProc)(widget_info *info);
typedef GUI_status (*GUI_EventProc)(widget_info *info, const SDL_Event *event);
typedef void (*GUI_FreeProc)(widget_info *info);


/* Create a GUI */
extern CGUI *GUI_Create(SDL_Surface *screen);

/* Create a generic widget */
extern CGUI_Widget *GUI_Widget_Create(void *data, int x, int y, int w, int h,
	GUI_DrawProc drawproc, GUI_EventProc eventproc, GUI_FreeProc freeproc);

/* Add a widget to a GUI.
   Once the widget has been added, it doesn't need to be freed.
   This function returns 0, or -1 if it ran out of memory.
 */
extern int GUI_AddWidget(CGUI *gui, CGUI_Widget *widget);

/* Move or resize a widget
   If any of the parameters are -1, that parameter is not changed.
 */
extern void GUI_MoveWidget(CGUI_Widget *widget, int x, int y, int w, int h);

/* Run a GUI until the widgets or idleproc request a quit */
extern void GUI_Run(CGUI *gui, GUI_IdleProc idle);

/* Delete a previously created GUI */
extern void GUI_Destroy(CGUI *gui);

#ifdef __cplusplus
};
#endif
#include "close_code.h"

#endif /* _GUI_C_h */
