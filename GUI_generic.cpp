/*
    GUILIB:  An example GUI framework library for use with SDL
*/

#include <stdio.h>
#include "GUI_generic.h"


GUI_GenericWidget:: GUI_GenericWidget(void *data, GUI_DrawProc drawproc,
			GUI_EventProc eventproc, GUI_FreeProc freeproc)
 : GUI_Widget(data)
{
	DrawProc = drawproc;
	EventProc = eventproc;
	FreeProc = freeproc;
}

GUI_GenericWidget:: ~GUI_GenericWidget()
{
	if ( FreeProc ) {
		widget_info info;

		FillInfo(&info);
		FreeProc(&info);
	}
}

void
GUI_GenericWidget:: FillInfo(widget_info *info)
{
	info->widget_data = widget_data;
	info->screen = screen;
	info->area = area;
}

void
GUI_GenericWidget:: Display(void)
{
	if ( DrawProc ) {
		widget_info info;

		FillInfo(&info);
		DrawProc(&info);
	}
}

GUI_status
GUI_GenericWidget:: HandleEvent(const SDL_Event *event)
{
	GUI_status status;

	status = GUI_PASS;
	if ( EventProc ) {
		int handle_it;

		/* Mouse events outside the widget area are ignored */
		handle_it = 1;
		switch (event->type) {
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP: {
				int x, y;
				x = event->button.x;
				y = event->button.y;
				if ( ! HitRect(x, y) ) {
					handle_it = 0;
				}
			}
			break;
			case SDL_MOUSEMOTION: {
				int x, y;
				x = event->motion.x;
				y = event->motion.y;
				if ( ! HitRect(x, y) ) {
					handle_it = 0;
				}
			}
			break;
		}
		if ( handle_it ) {
			widget_info info;

			FillInfo(&info);
			status = EventProc(&info, event);
		}
	}
	return(status);
}
