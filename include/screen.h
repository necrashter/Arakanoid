#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "sprite.h"
#include "entity.h"
#include "game_entities.h"
#include "load_resources.h"
#include "main.h"

// dynamic array
#include <vector>
// string stream
#include <sstream>

class Screen {
public:
	virtual void update(float dt){};
	virtual void render(){};
	virtual void handleEvent(SDL_Event &e){};
};

class GameScreen : public Screen {
public:
	GameScreen();
	void update(float dt);
	void render();
	void handleEvent(SDL_Event &e);
private:
	int lives=3;
	Sprite playerSprite;
	Sprite yellowBrick;
	Sprite shrinkPowerUp;
	Sprite ballSprite;
	Sprite bulletSprite;
	Sprite gunSprite;

	Bar playerEnt;

	std::vector<Ball> balls;
	std::vector<Brick> bricks;
	std::vector<PowerUp> powerups;
	std::vector<Bullet> bullets;
	std::vector<AnimatedSprite> animations;


	std::stringstream livesText;
	RenderedText livesRenderedText;
};


#endif
