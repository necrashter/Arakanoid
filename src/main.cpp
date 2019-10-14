#include "main.h"

#include "load_resources.h"
#include "sprite.h"
#include "entity.h"
#include "game_entities.h"
// old-school printf and fprintf
#include <cstdio>
// string stream
#include <sstream>
// dynamic array
#include <vector>
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

void quit() {
	free_resources();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

SDL_Rect redBrickRegion = {83,70,67,27};
SDL_Rect greenBrickRegion = {165,70,67,27};
SDL_Rect blueBrickRegion = {243,70,67,27};
SDL_Rect orangeBrickRegion = {243,277,67,27};

SDL_Rect gunRegion = {83,115,71,63};
SDL_Rect ballRegion = {165,115,71,63};
SDL_Rect growRegion = {243,115,71,63};

SDL_Rect smallBarRegion = {25,190,71,25};
SDL_Rect mediumBarRegion = {25,220,138,25};
SDL_Rect bigBarRegion = {25,252,204,25};

const vector_phys<phys_t> bulletVelocity(0,-360.0);

void game_loop(){
	bool running = true;

	SDL_Event e;

	Sprite playerSprite(spriteSheet, SDL_Rect{25,190,71,25}, SDL_Rect{40,440,71,25});

	Sprite yellowBrick(spriteSheet, SDL_Rect{5,70,67,27}, SDL_Rect{40,40,67,24});

	Sprite shrinkPowerUp(spriteSheet, SDL_Rect{5,115,71,63}, SDL_Rect{40,40,71,63});

	Sprite ballSprite(spriteSheet, SDL_Rect{285,202,14,14}, SDL_Rect{320-7,400-7,14,14});

	Sprite bulletSprite(spriteSheet, SDL_Rect{260,202,9,18}, SDL_Rect{320-7,400-7,9,18});
	Sprite gunSprite(spriteSheet, SDL_Rect{27,277,11,14}, SDL_Rect{320-7,400-7,11,14});

	//DynamicEntity testEnt(testSprite, vector_phys<phys_t>(60.f,60.f));
	int lives=3;
	Bar playerEnt(playerSprite, gunSprite);
	std::vector<Ball> balls;
	Ball ballEnt(ballSprite, vector_phys<phys_t>(180.f,180.f), &playerEnt);
	balls.push_back(ballEnt);
	std::vector<Brick> bricks;
	std::vector<PowerUp> powerups;
	std::vector<Bullet> bullets;
	srand(time(0)); //to prevent the same bonuses for every game
	for(int i=0;i<9;++i){
		for(int j=0;j<10;++j){
			Brick newBrick(yellowBrick, 20+i*67, 40+j*24);

			switch (rand()%20) { //reduced rate of bonuses
				case 0:
				newBrick.setTextureRegion(redBrickRegion);
				newBrick.type = gun;
				break;

				case 1:
				newBrick.setTextureRegion(greenBrickRegion);
				newBrick.type = ball;
				break;

				case 2:
				newBrick.setTextureRegion(blueBrickRegion);
				newBrick.type = enlarge;
				break;

				case 3:
				newBrick.setTextureRegion(orangeBrickRegion);
				newBrick.type = shrink;
				break;
			}
			bricks.push_back(newBrick);
		}
	}


	std::stringstream livesText;
	livesText.str("");
	livesText << "Lives: " << lives;
	RenderedText livesRenderedText(livesText.str(),regular_font, SDL_Color{255,255,255,255});

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
			} else if(e.type==SDL_KEYDOWN) {
				fprintf(stderr,"KEYDOWN: %d\n", e.key.keysym.sym);
			} else if(e.type==SDL_KEYUP) {
				fprintf(stderr,"KEYUP:   %d\n", e.key.keysym.sym);
			} else if(e.type==SDL_MOUSEBUTTONDOWN){
				if(!balls.empty())balls.begin()->throwBall();
				if(playerEnt.getHasGuns()){ // change to canShoot
					Bullet lb(bulletSprite,bulletVelocity);
					lb.setPosition(playerEnt.getLeftBulletPos());
					Bullet rb(bulletSprite,bulletVelocity);
					rb.setPosition(playerEnt.getRightBulletPos());
					bullets.push_back(lb);
					bullets.push_back(rb);
				}
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

					// before erasing the block, we need to add its PowerUp
					if(it->type!=none){
						PowerUp newPowerUp(shrinkPowerUp, vector_phys<phys_t>(0,99.9f));
						newPowerUp.setPosition(it->getHitbox().x-2, it->getHitbox().y);
						switch (newPowerUp.type=it->type) {
							case ball: newPowerUp.setTextureRegion(ballRegion);
							break;
							case gun: newPowerUp.setTextureRegion(gunRegion);
							break;
							case enlarge: newPowerUp.setTextureRegion(growRegion);
							break;
						}
						powerups.push_back(newPowerUp);
					}
					// erase invalidates the iterator
					// use returned iterator
					it = bricks.erase(it);
					if(bricks.empty()){
						printf("You won\n");
					}
				}else{
					++it;
				}
			}

			if(it2->isFallen()){
				// erase invalidates the iterator
				// use returned iterator
				it2 = balls.erase(it2);
				if(balls.empty()){
					lives--;
					livesText.str("");
					livesText << "Lives: " << lives;
					livesRenderedText.setString(livesText.str());
					if(lives<=0){
						printf("You lose\n");
					}else{
						Ball newBall(ballSprite, vector_phys<phys_t>(180.f,180.f), &playerEnt);
						balls.push_back(newBall);
					}
				}
			}else{
				++it2;
			}
		}

		auto it=powerups.begin();
		while(it!=powerups.end()){
			it->update(dt);
			if(playerEnt.checkCollision(*it)){
				if(it->type==ball && !balls.empty()){
					Ball newBall(ballSprite,
								vector_phys<phys_t>( 100.0f, -200.0f ),
								nullptr);
					Ball newBall2(ballSprite,
								vector_phys<phys_t>( -100.0f, -200.0f ),
								nullptr);
					auto pos = balls.begin()->getHitbox();
					newBall.setPosition(pos.x,pos.y);
					newBall2.setPosition(pos.x,pos.y);
					balls.push_back(newBall);
					balls.push_back(newBall2);
				}else if(it->type==enlarge && playerEnt.getSizeLevel()<2){
					playerEnt.changeSize(
						playerEnt.getSizeLevel()+1,
						playerEnt.getSizeLevel() == 0 ? mediumBarRegion : bigBarRegion
					);
				}else if(it->type==shrink && playerEnt.getSizeLevel()>0){
					playerEnt.changeSize(
						playerEnt.getSizeLevel()-1,
						playerEnt.getSizeLevel() == 2 ? mediumBarRegion : smallBarRegion
					);
				}else if(it->type==gun){
					playerEnt.arm();
				}
				it = powerups.erase(it);
			} else if(it->isFallen()){
				it = powerups.erase(it);
			} else {
				++it;
			}
		}

		auto it3=bullets.begin();
		while(it3!=bullets.end()){
			it3->update(dt);
			auto it2 = bricks.begin();
			while(it2!=bricks.end()){
				if(it3->checkCollision(*it2)){
					it3 = bullets.erase(it3);
					it2= bricks.erase(it2);
					if(bricks.empty()){
						printf("You won\n");
					}
					goto bulletsLoopEnd;
				}else{
					++it2;
				}
			}
			if(it3->isFallen()){
				it3 = bullets.erase(it3);
			} else {
				++it3;
			}
			bulletsLoopEnd:;
		}

		SDL_SetRenderDrawColor(renderer, 56, 60, 74, 255);
		SDL_RenderClear(renderer); // clear the screen

		playerEnt.render();
		//ballEnt.render();
		for(auto it=powerups.begin();it!=powerups.end();++it)it->render();
		for(auto it=bricks.begin();it!=bricks.end();++it)it->render();

		for(auto it=bullets.begin();it!=bullets.end();++it)it->render();
		for(auto it=balls.begin();it!=balls.end();++it)it->render();

		livesRenderedText.render();

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
