#ifndef _GAME_ENTITIES_H_
#define _GAME_ENTITIES_H_

#include "entity.h"

extern const float MAX_BAR_SPEED;
extern const float BAR_ACCELERATION;

class Bar: public DynamicEntity{
public:
	Bar(Sprite sprite_arg);
	void update(float delta);
};

class Ball: public DynamicEntity{
public:
	Ball(Sprite sprite_arg,vector_phys<phys_t> speed_arg);
	void update(float delta);
	void collision(Entity&);  //if horizontal  Ball.speed===negative_horizon, if vertical Ball.speed===negative_vertical (according to implemention of negative funcs. )
	void barCollision(Bar&);
};

class Brick: public Entity{
public:
	Brick(Sprite sprite_arg, phys_t x, phys_t y);
	void breaking();
};



#endif
