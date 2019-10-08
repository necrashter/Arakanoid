#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "sprite.h"

typedef float phys_t; // type of position, velocity etc.

template <class T>
class vector_phys{
	T x;
	T y;
public:
	vector_phys(T a=0,T b=0){
		x=a;
		y=b;
	}
	T get_x(){return x;}
	T get_y(){return y;}
	vector_phys operator -(){
		vector_phys res;
		res.x=-x;
		res.y=-y;
		return res;
	}
	vector_phys operator *(T number){
		vector_phys res;
		res.x=x*number;
		res.y=y*number;
		return res;
	}
	vector_phys operator +(vector_phys lhs){
		vector_phys res;
		res.x=x+lhs.x;
		res.y=y+lhs.y;
		return res;
	}
	vector_phys negative_horizon(){
		vector_phys res;
		res.x=-x;
		res.y=y;
		return res;
	}
	vector_phys negative_vertical(){
		vector_phys res;
		res.x=x;
		res.y=-y;
		return res;
	}


};

class Entity{
protected:
	Sprite sprite_entity;
	vector_phys<phys_t> position; //position

public:
	Entity(Sprite sprite_arg,vector_phys<phys_t> position_arg):sprite_entity(sprite_arg),position(position_arg){}
	virtual  void collision();
	Sprite getSprite_entity () const{ return sprite_entity; }

	void setSprite_entity(SDL_Texture* texture_arg,
	SDL_Rect textureRegion_arg,
	SDL_Rect position_arg){
		sprite_entity.setTexture(texture_arg);
		sprite_entity.setTextureRegion(textureRegion_arg);
		sprite_entity.setPosition(position_arg);
	}

	void setPosition(vector_phys<phys_t> position_arg){position=position_arg;}
	vector_phys<phys_t> getPosition() const {return position;}
};

class DynamicEntity: public Entity{
protected:
	vector_phys<phys_t> speed;
public:
	DynamicEntity(Sprite sprite_arg,vector_phys<phys_t> position_arg,vector_phys<phys_t> speed_arg):Entity(sprite_arg,position_arg),speed(speed_arg){}
	virtual void update(float delta){
		position=position+speed*delta;
	}

};

class Ball: public DynamicEntity{
public:
	Ball(Sprite sprite_arg,vector_phys<phys_t> position_arg,vector_phys<phys_t> speed_arg):DynamicEntity(sprite_arg,position_arg,speed_arg){}
	void collision(); //if horizontal  Ball.speed===negative_horizon, if vertical Ball.speed===negative_vertical (according to implemention of negative funcs. )
};

class Brick: public Entity{
public:
	void breaking();
	void collision(){breaking();}
};

class Bar: public DynamicEntity{};

#endif
