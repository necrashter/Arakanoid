#include "main.h"

#include "load_resources.h"
#include "sprite.h"
#include "entity.h"
#include "game_entities.h"
// old-school printf and fprintf
#include <cstdio>
// string stream
#include <sstream>
#include <vector>

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

	Sprite playerSprite(spriteSheet, SDL_Rect{25,190,71,25}, SDL_Rect{40,440,71,25});
	Sprite yellowBrick(spriteSheet, SDL_Rect{5,70,67,27}, SDL_Rect{40,40,67,24});
	Sprite ballSprite(spriteSheet, SDL_Rect{285,202,14,14}, SDL_Rect{320-7,400-7,14,14});

	//DynamicEntity testEnt(testSprite, vector_phys<phys_t>(60.f,60.f));
	static int lives=3;
	Bar playerEnt(playerSprite);
	std::vector<Ball> balls;
	Ball ballEnt(ballSprite, vector_phys<phys_t>(180.f,180.f), &playerEnt);
	balls.push_back(ballEnt);
	std::vector<Brick> bricks;

	for(int i=0;i<9;++i){
		for(int j=0;j<10;++j){
			Brick newBrick(yellowBrick, 20+i*67, 40+j*24);
			bricks.push_back(newBrick);
		}
	}


	RenderedText testText("Hello World!",regular_font, SDL_Color{255,255,255,255});

	Uint32 previousFrame = -MILLISECONDS_PER_FRAME;
	Uint32 frameStart;
	std::stringstream fpsText;
	while(running){
		frameStart = SDL_GetTicks();
		// Get the time passed since last frame
		// IN MILLISECONDS

		float dt = ((float)(frameStart - previousFrame)) / 1000.0f; // delta time in seconds

		while(SDL_PollEvent(&e)!=0){
			if(e.type==SDL_QUIT){
				running=false;
			} else if(e.type==SDL_KEYDOWN) {
				fprintf(stderr,"KEYDOWN: %d\n", e.key.keysym.sym);
			} else if(e.type==SDL_KEYUP) {
				fprintf(stderr,"KEYUP:   %d\n", e.key.keysym.sym);
			} else if(e.type==SDL_MOUSEBUTTONDOWN){
				ballEnt.throwBall();
			}
		} // end event handling

		playerEnt.update(dt);
		//ballEnt.update(dt);


		auto it2=balls.begin();
		while(it2!=balls.end()){
			it2->update(dt);
			if(playerEnt.checkCollision(*it2)){
				it2->barCollision(playerEnt);
			}
			auto it=bricks.begin();
			while(it!=bricks.end()){
				if(it->checkCollision(*it2)){
					it2->collision((*it));
					// erase invalidates the iterator
					// use returned iterator
					it = bricks.erase(it);
				}else{
					++it;
				}
			}

			if(bricks.empty()) /*win */ ;
			if(balls.empty()) lives--;
			if(lives==0) /*end game*/;



			if(hitbox.y>SCREEN_HEIGHT /*bottom*/ && velocity.y>0){
				// erase invalidates the iterator
				// use returned iterator
				it2 = balls.erase(it2);
			}else{
				++it2;
			}
		}

		/*auto it=bricks.begin();
		while(it!=bricks.end()){
			if(it->checkCollision(ballEnt)){
				ballEnt.collision((*it));
				// erase invalidates the iterator
				// use returned iterator
				it = bricks.erase(it);
			}else{
				++it;
			}
		}*/



		if(playerEnt.checkCollision(/*ExtraBall t*/){
			for(int i=0;i<2;i++){
				Ball newBall(ballSprite, vector_phys<phys_t>(180.f*pow(-1,i),180.f), &playerEnt);// they should split up into three directions
				balls.push_back(newBall);
			}
		}

		if(playerEnt.checkCollision(/*Laser*/){
		}
		if(playerEnt.checkCollision(/*Enlarge*/){

		}
		if(playerEnt.checkCollision(/*ExtraBall t*/){
		}

		fpsText.str("");
		fpsText << "FPS: " << 1.0f/dt;
		testText.setString(fpsText.str());

		//SDL_BlitScaled( gHelloWorld, NULL, screenSurface, &screenRect );
		//SDL_UpdateWindowSurface( window ); // call after every blit

		SDL_SetRenderDrawColor(renderer, 56, 60, 74, 255);
		SDL_RenderClear(renderer); // clear the screen

		// first null is srcrect, which is the entire texture
		// seconde null is dstrect, which is the entire screen
		//SDL_RenderCopy(renderer,testTexture, NULL, NULL);

		playerEnt.render();
		//ballEnt.render();
		for(auto it=bricks.begin();it!=bricks.end();++it)it->render();
		for(auto it=balls.begin();it!=balls.end();++it)it->render();

		testText.render();

		SDL_RenderPresent(renderer); // update

		previousFrame = SDL_GetTicks();
		Uint32 frameTime = SDL_GetTicks()-frameStart;
		if(frameTime<MILLISECONDS_PER_FRAME){
			SDL_Delay(MILLISECONDS_PER_FRAME-frameTime);
		}
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
