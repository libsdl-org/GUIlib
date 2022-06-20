/*
    GUILIB:  An example GUI framework library for use with SDL
*/

/* This is the most generic widget possible, -- it's exported for C functions
 */

#ifndef _GUI_generic_h
#define _GUI_generic_h

#include "GUI_C.h"
#include "GUI_widget.h"


class GUI_GenericWidget : public GUI_Widget {

public:
	GUI_GenericWidget(void *data, GUI_DrawProc drawproc,
			GUI_EventProc eventproc, GUI_FreeProc freeproc);
	~GUI_GenericWidget();

	/* Show the widget.
	   If the surface needs to be locked, it will be locked
	   before this call, and unlocked after it returns.
	 */
	virtual void Display(void);

	/* Main event handler function.
	   This function gets raw SDL events from the GUI.
	 */
	virtual GUI_status HandleEvent(const SDL_Event *event);

protected:
	GUI_DrawProc DrawProc;
	GUI_EventProc EventProc;
	GUI_FreeProc FreeProc;

	/* Fill a widget_info structure with our info */
	virtual void FillInfo(widget_info *info);
};

#endif /* _GUI_generic_h */
