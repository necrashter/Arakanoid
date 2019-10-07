#include "main.h"

#include "load_resources.h"
#include <cstdio>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer = NULL;

bool sdl_init() {
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL_Init failed! SDL_Error: %s\n", SDL_GetError() );
    }else{
        window = SDL_CreateWindow( "Arakanoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL ) {
            printf( "SDL_CreateWindow failed! SDL_Error: %s\n", SDL_GetError() );
        }
        else {
			// init SDL_Renderer
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if(renderer == NULL){
				fprintf(stderr, "Cannot create renderer: %s\n", SDL_GetError());
			}else{
                //Initialize renderer color
                SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

				// Initialize SDL_image
				int image_flags = IMG_INIT_PNG; // we need to load some pngs
				if( !(IMG_Init(image_flags)&&image_flags)){
					printf( "IMG_Init failed! SDL_Error: %s\n", SDL_GetError() );
				} else {
					screenSurface = SDL_GetWindowSurface( window );
					return true;
				}
			}
        }
    }
	return false;
}

void quit() {
	free_resources();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
}

void game_loop(){
	bool running = true;

	SDL_Event e;

	//SDL_Rect screenRect;
	//screenRect.x = 0;
	//screenRect.y = 0;
	//screenRect.w = SCREEN_WIDTH;
	//screenRect.h = SCREEN_HEIGHT;
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

		//SDL_BlitScaled( gHelloWorld, NULL, screenSurface, &screenRect );
		//SDL_UpdateWindowSurface( window ); // call after every blit

		SDL_RenderClear(renderer); // clear the screen

		// first null is srcrect, which is the entire texture
		// seconde null is dstrect, which is the entire screen
		SDL_RenderCopy(renderer,testTexture, NULL, NULL);

		SDL_RenderPresent(renderer); // update
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
