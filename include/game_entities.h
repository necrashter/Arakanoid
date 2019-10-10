#ifndef _GAME_ENTITIES_H_
#define _GAME_ENTITIES_H_

#include "entity.h"

const float MAX_BAR_SPEED =360.0f;

class Ball: public DynamicEntity{
public:
	Ball(Sprite sprite_arg,vector_phys<phys_t> speed_arg):
		DynamicEntity(sprite_arg,speed_arg){}
	void update(float delta);
	void collision(); //if horizontal  Ball.speed===negative_horizon, if vertical Ball.speed===negative_vertical (according to implemention of negative funcs. )
};

class Brick: public Entity{
public:
	void breaking();
	void collision(){breaking();}
};

class Bar: public DynamicEntity{
public:
	Bar(Sprite sprite_arg);
	void update(float delta);
};

#endif
