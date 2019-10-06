#ifndef _MAIN_H_
#define _MAIN_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

extern const int SCREEN_WIDTH = 640;
extern const int SCREEN_HEIGHT = 480;

// we need to declare (but not define)
// hence extern
extern SDL_Window* window;

//The surface contained by the window
extern SDL_Surface* screenSurface;


bool sdl_init();
void quit();

// main game loop
void game_loop();

#endif
