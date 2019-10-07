#include "load_resources.h"
#include "main.h"

SDL_Texture* testTexture = NULL;

SDL_Surface* load_surface( std::string path ){
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL ){
        printf( "Cannot load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else{
        optimizedSurface = SDL_ConvertSurface( loadedSurface, screenSurface->format, 0 );
        if( optimizedSurface == NULL ){
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
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
        printf( "Cannot load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else{
        output = SDL_CreateTextureFromSurface(renderer,loadedSurface);
        if( output == NULL ){
            printf( "Unable to create texture %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        SDL_FreeSurface( loadedSurface );
    }

    return output;
}

void load_resources() {
	testTexture = load_texture("gfx/test.png");
}

void free_resources() {
	SDL_DestroyTexture(testTexture);
}
