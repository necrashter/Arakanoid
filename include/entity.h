#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "sprite.h"
#include "phys.h"

class Entity{
protected:
	Sprite sprite_entity;
	box_phys<phys_t> hitbox;

public:
	Entity(Sprite sprite_arg);

	Sprite getSprite_entity () const{ return sprite_entity; }

	void setSprite_entity(SDL_Texture* texture_arg,
	SDL_Rect textureRegion_arg,
	SDL_Rect position_arg);

	virtual bool checkCollision(const Entity& other);
	void render();

	void setPosition(vector_phys<phys_t> position_arg){
		hitbox.x = position_arg.x;
		hitbox.y = position_arg.y;
		sprite_entity.setPosition(hitbox.x,hitbox.y);
	}
	void setPosition(phys_t x, phys_t y){
		hitbox.x = x;
		hitbox.y = y;
		sprite_entity.setPosition(x,y);
	}
	box_phys<phys_t> getHitbox() const {return hitbox;}
};

class DynamicEntity: public Entity{
protected:
	vector_phys<phys_t> speed;
public:
	DynamicEntity(Sprite sprite_arg,
		vector_phys<phys_t> speed_arg):
		Entity(sprite_arg),speed(speed_arg){}
	virtual void update(float delta);

};

class Ball: public DynamicEntity{
public:
	Ball(Sprite sprite_arg,vector_phys<phys_t> speed_arg):
	DynamicEntity(sprite_arg,speed_arg){}
	void collision(); //if horizontal  Ball.speed===negative_horizon, if vertical Ball.speed===negative_vertical (according to implemention of negative funcs. )
};

class Brick: public Entity{
public:
	void breaking();
	void collision(){breaking();}
};

class Bar: public DynamicEntity{};

#endif
