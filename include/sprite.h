#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Sprite {
private:
	SDL_Texture* texture;
	SDL_Rect textureRegion;
	SDL_Rect position;
public:
	Sprite(SDL_Texture* texture_arg,
	SDL_Rect textureRegion_arg,
	SDL_Rect position_arg){
		texture=texture_arg;
		textureRegion=textureRegion_arg;
		position=position_arg;
	}

	// destructor should not destoy the texture,
	// it will be handled on game exit
	~Sprite();

	// TODO: Write getter and setters for all private members
	// getters and setters can be defined right here,
	// as we want these functions to be inlined
	// don't forget to make getters const

	//const SDL_Rect& getTextureRegion() const { return textureRegion; }
	SDL_Rect getTextureRegion() const { return textureRegion; }
	void setTextureRegion(SDL_Rect& a) { textureRegion = a; }
	 SDL_Rect getPosition() const{ return position; }
	void setPosition(SDL_Rect& a) { position = a; }
	 SDL_Texture* getTexture() const{return texture;}
	void setTexture(SDL_Texture* a) { texture = a; }
};
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
		res.y=y*lhs.y;
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
	vector_phys position; //position

public:
	Entity(Sprite sprite_arg,vector_phys position_arg):sprite_entity(sprite_arg),position(position_arg){}
	virtual  void collision();
	Sprite getSprite_entity () const{
		Sprite res;
		res.setTexture(sprite_entity.getTexture());
		res.setTextureRegion(sprite_entity.getTextureRegion());
		res.setPosition(sprite_entity.getPosition());
		return res;
	}
	void setSprite_entity(SDL_Texture* texture_arg,
	SDL_Rect textureRegion_arg,
	SDL_Rect position_arg){
		sprite_entity.setTexture(texture_arg);
		sprite_entity.getTextureRegion(textureRegion_arg);
		sprite_entity.getPosition(position_arg);
	}
	void setPosition(vector_phys position_arg){position=position_arg;}
	vector_phys getPosition() const {return position;}
};

class DynamicEntity:public Entity{
protected:
	vector_phys speed;
public:
	DynamicEntity(Sprite sprite_arg,vector_phys position_arg,vector_phys speed_arg):Entity(sprite_arg,position_arg),speed(speed_arg){}
	virtual void update(float delta);

};

class Ball:public DynamicEntity{
public:
	Ball(Sprite sprite_arg,vector_phys position_arg,vector_phys speed_arg):DynamicEntity(sprite_arg,position_arg,speed_arg){}
	collision(); //if horizontal  Ball.speed===negative_horizon, if vertical Ball.speed===negative_vertical (according to implemention of negative funcs. )
};

class Brick:public Entity{
public:
	void breaking();
	void collision(){breaking();}
};

class Bar:public DynamicEntity{};

#endif
