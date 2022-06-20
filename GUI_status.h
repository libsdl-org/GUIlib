/*
    GUILIB:  An example GUI framework library for use with SDL
*/

/* These are return codes for widget event functions */

#ifndef _GUI_status_h
#define _GUI_status_h

typedef enum {
	GUI_QUIT,			/* Close the GUI */
	GUI_REDRAW,			/* The GUI needs to be redrawn */
	GUI_YUM,			/* The event was eaten by the widget */
	GUI_PASS			/* The event should be passed on */
} GUI_status;

typedef enum {
	WIDGET_VISIBLE,
	WIDGET_HIDDEN,
	WIDGET_DELETED
} WIDGET_status;

/* GUI idle function -- run when no events pending */
typedef GUI_status (*GUI_IdleProc)(void);

#endif /* _GUI_status_h */
