
/* Program to pop up a quick "OK" message box  */

#include "SDL.h"
#include "GUI_output.h"


int main(int argc, char *argv[])
{
	SDL_Window *window;

	if ( argv[1] == NULL ) {
		fprintf(stderr, "Usage: %s <message>\n", argv[0]);
		exit(1);
	}
		
	/* Initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	/* Get a video mode for display */
	window = SDL_CreateWindow("Hi there!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 200, 0);

	/* Pop up the message and quit */
	GUI_MessageBox(window, "Message", argv[1], GUI_MBOK);
	exit(0);
}
