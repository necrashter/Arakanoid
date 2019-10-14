#include "load_resources.h"
#include "main.h"

SDL_Texture* brickExplosionTexture = NULL;
SDL_Texture* spriteSheet = NULL;
TTF_Font* regular_font = NULL;

SDL_Surface* load_surface( std::string path ){
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL ){
        fprintf(stderr, "Cannot load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else{
        optimizedSurface = SDL_ConvertSurface( loadedSurface, screenSurface->format, 0 );
        if( optimizedSurface == NULL ){
            fprintf(stderr, "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}

SDL_Texture* load_texture( std::string path ){
    //The final optimized image
    SDL_Texture* output = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL ){
        fprintf(stderr, "Cannot load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else{
        output = SDL_CreateTextureFromSurface(renderer,loadedSurface);
        if( output == NULL ){
            fprintf(stderr, "Unable to create texture %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        SDL_FreeSurface( loadedSurface );
    }

    return output;
}

void load_resources() {
	brickExplosionTexture = load_texture("gfx/brick_explode.png");
	spriteSheet = load_texture("gfx/sh_2.png");
	regular_font = TTF_OpenFont("fonts/Grenze-Regular.ttf", 32 );
}

void free_resources() {
    TTF_CloseFont(regular_font);
	SDL_DestroyTexture(brickExplosionTexture);
	SDL_DestroyTexture(spriteSheet);
}
