#ifndef _MAIN_H_
#define _MAIN_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "screen.h"

extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;

// we need to declare (but not define)
// hence extern
extern SDL_Window* window;

//The surface contained by the window
extern SDL_Surface* screenSurface;
// renderer for window, hardware rendering
extern SDL_Renderer* renderer;
extern SDL_Rect* brickExplosionFrames;

extern Screen* currentScreen;

bool sdl_init();
void quit();

// main game loop
void game_loop();

// change the GameScreen while running
void set_screen(GameScreen* newScreen);

#endif
