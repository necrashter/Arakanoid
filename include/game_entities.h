#ifndef _GAME_ENTITIES_H_
#define _GAME_ENTITIES_H_

#include "entity.h"

extern const float MAX_BAR_SPEED;
extern const float BAR_ACCELERATION;

enum PowerUpType {
	none,
	ball,
	gun,
	enlarge,
	shrink
};

class Bar: public DynamicEntity{
public:
	Bar(Sprite sprite_arg);
	void update(float delta);
};

class Ball: public DynamicEntity{
private:
	Bar* bar;
public:
	Ball(Sprite sprite_arg,vector_phys<phys_t> speed_arg, Bar* parent);
	void update(float delta);
	void collision(Entity&);  //if horizontal  Ball.speed===negative_horizon, if vertical Ball.speed===negative_vertical (according to implemention of negative funcs. )
	void barCollision(Bar&);
	void throwBall();
};

class Brick: public Entity{
public:
	PowerUpType type;
	Brick(Sprite sprite_arg, phys_t x, phys_t y);
	void breaking();
};

class PowerUp: public DynamicEntity{
public:
	PowerUpType type;
	PowerUp(Sprite sprite_arg,vector_phys<phys_t> speed_arg):DynamicEntity(sprite_arg,speed_arg){}
};


#endif
