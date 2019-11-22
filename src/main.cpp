#include "main.h"

#include "load_resources.h"
#include "sprite.h"
#include "entity.h"
#include "game_entities.h"
#include "screen.h"
// old-school printf and fprintf
#include <cstdio>
// for random
#include <stdlib.h>
#include<ctime>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MILLISECONDS_PER_FRAME = 1000/60;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer = NULL;

bool sdl_init() {
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		fprintf(stderr, "SDL_Init failed! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}
	window = SDL_CreateWindow( "Arakanoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if( window == NULL ) {
		fprintf(stderr, "SDL_CreateWindow failed! SDL_Error: %s\n", SDL_GetError() );

		return false;
	}
	// init SDL_Renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){
		fprintf(stderr, "Cannot create renderer: %s\n", SDL_GetError());
		return false;
	}

	// Initialize SDL_image
	int image_flags = IMG_INIT_PNG; // we need to load some pngs
	if( !(IMG_Init(image_flags)&&image_flags)){
		fprintf(stderr, "IMG_Init failed! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}

	if(TTF_Init() == -1){
		fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}

	screenSurface = SDL_GetWindowSurface( window );
	return true;
}


SDL_Rect* brickExplosionFrames = nullptr;

void loadFrames(){
	brickExplosionFrames=new SDL_Rect[12]();
	for(int i=0;i<12;++i){
		brickExplosionFrames[i] = {(i%4)*160,(i/4)*154,160,154};
	}
}

void quit() {
	delete[] brickExplosionFrames;
	free_resources();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

void game_loop(){
	bool running = true;

	SDL_Event e;

	srand(time(0)); //to prevent the same bonuses for every game

	Screen *currentScreen = new GameScreen();

	Uint32 previousFrame = -MILLISECONDS_PER_FRAME;
	Uint32 frameStart;
	while(running){
		frameStart = SDL_GetTicks();
		// Get the time passed since last frame
		// IN MILLISECONDS

		float dt = ((float)(frameStart - previousFrame)) / 1000.0f; // delta time in seconds

		while(SDL_PollEvent(&e)!=0){
			if(e.type==SDL_QUIT){
				running=false;
			} else currentScreen->handleEvent(e);
		} // end event handling

		currentScreen->update(dt);

		currentScreen->render();

		SDL_RenderPresent(renderer); // update

		previousFrame = SDL_GetTicks();
		Uint32 frameTime = SDL_GetTicks()-frameStart;
		if(frameTime<MILLISECONDS_PER_FRAME){
			SDL_Delay(MILLISECONDS_PER_FRAME-frameTime);
		}
	} // end game loop
	delete currentScreen;
}


int main( int argc, char* args[] ) {
	if(!sdl_init()){
		fprintf(stderr,"SDL couldn't initialize.\n");
	}else{
		load_resources();
		loadFrames();
		game_loop();
	}
	quit();
    return 0;
}
