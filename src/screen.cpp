#include "screen.h"

#include "main.h"

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




GameScreen::GameScreen():
playerSprite(spriteSheet, SDL_Rect{25,190,71,25}, SDL_Rect{40,440,71,25}),
yellowBrick(spriteSheet, SDL_Rect{5,70,67,27}, SDL_Rect{40,40,67,24}),
shrinkPowerUp(spriteSheet, SDL_Rect{5,115,71,63}, SDL_Rect{40,40,71,63}),
ballSprite(spriteSheet, SDL_Rect{285,202,14,14}, SDL_Rect{320-7,400-7,14,14}),
bulletSprite(spriteSheet, SDL_Rect{260,202,9,18}, SDL_Rect{320-7,400-7,9,18}),
gunSprite(spriteSheet, SDL_Rect{27,277,11,14}, SDL_Rect{320-7,400-7,11,14}),
playerEnt(playerSprite, gunSprite),
livesText(),
livesRenderedText("",regular_font, SDL_Color{255,255,255,255}),
balls(), bricks(), powerups(), bullets(), animations()
{
	Ball ballEnt(ballSprite, vector_phys<phys_t>(180.f,180.f), &playerEnt);
	balls.push_back(ballEnt);
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
	livesText.str("");
	livesText << "Lives: " << lives;
	livesRenderedText.setString(livesText.str());
}

void GameScreen::update(float dt){
	playerEnt.update(dt);

	auto animit=animations.begin();
	while(animit!=animations.end()){
		if(animit->update(dt))animit = animations.erase(animit);
		else ++animit;
	}
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
				// add the animation
				AnimatedSprite testAnim(brickExplosionTexture,brickExplosionFrames,12,it->getHitbox().x-46,it->getHitbox().y-60);
				animations.push_back(testAnim);
				// erase invalidates the iterator
				// use returned iterator
				it = bricks.erase(it);
				if(bricks.empty()){
					//printf("You won\n");
					set_screen((GameScreen*)new RestartScreen());
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
					//printf("You lose\n");
					set_screen((GameScreen*)new RestartScreen());
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
				// powerups
				if(it2->type!=none){
					PowerUp newPowerUp(shrinkPowerUp, vector_phys<phys_t>(0,99.9f));
					newPowerUp.setPosition(it2->getHitbox().x-2, it2->getHitbox().y);
					switch (newPowerUp.type=it2->type) {
						case ball: newPowerUp.setTextureRegion(ballRegion);
						break;
						case gun: newPowerUp.setTextureRegion(gunRegion);
						break;
						case enlarge: newPowerUp.setTextureRegion(growRegion);
						break;
					}
					powerups.push_back(newPowerUp);
				}


				it2= bricks.erase(it2);
				if(bricks.empty()){
					//printf("You won\n");
					set_screen((GameScreen*)new RestartScreen());
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
}

void GameScreen::render(){
	SDL_SetRenderDrawColor(renderer, 56, 60, 74, 255);
	SDL_RenderClear(renderer); // clear the screen

	playerEnt.render();
	//ballEnt.render();
	for(auto it=powerups.begin();it!=powerups.end();++it)it->render();
	for(auto it=bricks.begin();it!=bricks.end();++it)it->render();

	for(auto it=bullets.begin();it!=bullets.end();++it)it->render();
	for(auto it=balls.begin();it!=balls.end();++it)it->render();

	for(auto it=animations.begin();it!=animations.end();++it)it->render();

	livesRenderedText.render();
}

void GameScreen::handleEvent(SDL_Event &e){
	if(e.type==SDL_KEYDOWN) {
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
}


// Start Screen
StartScreen::StartScreen () :
	menuText("PRESS ENTER TO START",regular_font, SDL_Color{255,255,255,255}) {

}

void StartScreen::update(float dt) {
	// nothing?
}

void StartScreen::render() {
	SDL_SetRenderDrawColor(renderer, 56, 60, 74, 255);
	SDL_RenderClear(renderer); // clear the screen
	menuText.render();
}

void StartScreen::handleEvent(SDL_Event &e){
	if(e.type==SDL_KEYUP) {
		if(e.key.keysym.sym == SDLK_RETURN){
			set_screen(new GameScreen());
		}
	}
}

// Restart Screen
RestartScreen::RestartScreen () :
	menuText("PRESS ENTER TO RESTART",regular_font, SDL_Color{255,255,255,255}) {

}

void RestartScreen::update(float dt) {
	// nothing?
}

void RestartScreen::render() {
	SDL_SetRenderDrawColor(renderer, 56, 60, 74, 255);
	SDL_RenderClear(renderer); // clear the screen
	menuText.render();
}

void RestartScreen::handleEvent(SDL_Event &e){
	if(e.type==SDL_KEYUP) {
		if(e.key.keysym.sym == SDLK_RETURN){
			set_screen(new GameScreen());
		}
	}
}
