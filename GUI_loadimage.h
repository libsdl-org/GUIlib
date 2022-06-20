/*
    GUILIB:  An example GUI framework library for use with SDL
*/

#ifndef _GUI_loadimage_h
#define _GUI_loadimage_h

#include "SDL.h"
#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/* C functions for C image loading support:
   Use 'genimage' to turn an 8-bit BMP file into a C image, and include
   the output in your application, either directly or as a header file.
   Create an SDL surface in your program:
	surface = GUI_LoadImage(image_w, image_h, image_pal, image_data);
 */
/************************************************************************/

extern SDL_Surface *GUI_LoadImage(int w, int h, Uint8 *pal, Uint8 *data);

/* Load the internal 8x8 font and return the associated font surface */
extern SDL_Surface *GUI_DefaultFont(void);

#ifdef __cplusplus
};
#endif
#include "close_code.h"

#endif /* _GUI_loadimage_h */
