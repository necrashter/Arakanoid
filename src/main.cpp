#include "main.h"

#include "load_resources.h"
#include "sprite.h"
// old-school printf and fprintf
#include <cstdio>
// string stream
#include <sstream>

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

void quit() {
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

	//SDL_Rect screenRect;
	//screenRect.x = 0;
	//screenRect.y = 0;
	//screenRect.w = SCREEN_WIDTH;
	//screenRect.h = SCREEN_HEIGHT;
	Sprite playerSprite(spriteSheet, SDL_Rect{25,190,71,25}, SDL_Rect{40,440,71,25});
	Sprite testSprite(spriteSheet, SDL_Rect{5,70,67,27}, SDL_Rect{40,40,67,24});
	RenderedText testText("Hello World!",regular_font, SDL_Color{255,255,255,255});

	Uint32 previousFrame = 0;
	std::stringstream fpsText;
	while(running){
		// Get the time passed since last frame
		// IN MILLISECONDS
		Uint32 delta = SDL_GetTicks() - previousFrame;
		if(delta<MILLISECONDS_PER_FRAME)continue;
		previousFrame = SDL_GetTicks();

		while(SDL_PollEvent(&e)!=0){
			if(e.type==SDL_QUIT){
				running=false;
			} else if(e.type==SDL_KEYDOWN) {
				fprintf(stderr,"KEYDOWN: %d\n", e.key.keysym.sym);
			} else if(e.type==SDL_KEYUP) {
				fprintf(stderr,"KEYUP:   %d\n", e.key.keysym.sym);
			}
		} // end event handling

		fpsText.str("");
		fpsText << "FPS: " << 1000.0f/delta;
		testText.setString(fpsText.str());

		SDL_Rect newPos = playerSprite.getPosition();
		int mousex,mousey;
		SDL_GetMouseState(&mousex,&mousey);
		newPos.x=mousex;
		playerSprite.setPosition(newPos);

		//SDL_BlitScaled( gHelloWorld, NULL, screenSurface, &screenRect );
		//SDL_UpdateWindowSurface( window ); // call after every blit

		SDL_SetRenderDrawColor(renderer, 122, 122, 121, 255);
		SDL_RenderClear(renderer); // clear the screen

		// first null is srcrect, which is the entire texture
		// seconde null is dstrect, which is the entire screen
		//SDL_RenderCopy(renderer,testTexture, NULL, NULL);

		testSprite.render();
		playerSprite.render();

		testText.render();

		SDL_RenderPresent(renderer); // update
	} // end game loop
}


int main( int argc, char* args[] ) {
	if(!sdl_init()){
		fprintf(stderr,"SDL couldn't initialize.\n");
	}else{
		load_resources();
		game_loop();
	}
	quit();
    return 0;
}
