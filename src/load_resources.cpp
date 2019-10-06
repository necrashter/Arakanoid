#include "load_resources.h"
#include "main.h"

SDL_Surface* gHelloWorld = NULL;

SDL_Surface* load_surface( std::string path )
{
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

void load_resources() {
    gHelloWorld = load_surface( "gfx/test.png" );
}

void free_resources() {
	SDL_FreeSurface(gHelloWorld);
}
