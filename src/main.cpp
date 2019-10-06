#include "main.h"

#include "load_resources.h"
#include <cstdio>

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;

bool sdl_init() {
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL_Init failed! SDL_Error: %s\n", SDL_GetError() );
    }else{
        window = SDL_CreateWindow( "Arakanoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL ) {
            printf( "SDL_CreateWindow failed! SDL_Error: %s\n", SDL_GetError() );
        }
        else {
			// Initialize SDL_image
			int image_flags = IMG_INIT_PNG;
			if( !(IMG_Init(image_flags)&&image_flags)){
				printf( "IMG_Init failed! SDL_Error: %s\n", SDL_GetError() );
			} else {
            screenSurface = SDL_GetWindowSurface( window );
			return true;
			}
        }
    }
	return false;
}

void quit() {
	free_resources();
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void game_loop(){
	bool running = true;

	SDL_Event e;

	SDL_Rect screenRect;
	screenRect.x = 0;
	screenRect.y = 0;
	screenRect.w = SCREEN_WIDTH;
	screenRect.h = SCREEN_HEIGHT;
	while(running){
		while(SDL_PollEvent(&e)!=0){
			if(e.type==SDL_QUIT){
				running=false;
			} else if(e.type==SDL_KEYDOWN) {
				printf("KEYDOWN: %d\n", e.key.keysym.sym);
			} else if(e.type==SDL_KEYUP) {
				printf("KEYUP:   %d\n", e.key.keysym.sym);
			}
		} // end event handling

		SDL_BlitScaled( gHelloWorld, NULL, screenSurface, &screenRect );

		SDL_UpdateWindowSurface( window ); // call after every blit
	} // end game loop
}


int main( int argc, char* args[] ) {
	if(!sdl_init()){
		printf("SDL couldn't initialize.\n");
	}else{
		load_resources();
		game_loop();
	}
	quit();
    return 0;
}
