#ifndef _LOAD_RESOURCES_H_
#define _LOAD_RESOURCES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

extern SDL_Texture* testTexture;
extern SDL_Texture* spriteSheet;

SDL_Surface* load_surface(std::string path);
SDL_Texture* load_texture(std::string path);

void load_resources();
void free_resources();

#endif
