#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "sprite.h"
#include "phys.h"

class Entity{
protected:
	Sprite sprite_entity;
	box_phys<phys_t> hitbox;
	virtual void positionUpdated();
public:
	Entity(Sprite sprite_arg);

	Sprite getSprite_entity () const{ return sprite_entity; }

	void setSprite_entity(SDL_Texture* texture_arg,
	SDL_Rect textureRegion_arg,
	SDL_Rect position_arg);
	void setTextureRegion(SDL_Rect reg);

	virtual bool checkCollision(const Entity& other);
	bool checkHorizontalCollision(const Entity& other);
	bool checkVerticalCollision(const Entity& other);
	virtual void render();

	void setPosition(vector_phys<phys_t> position_arg){
		hitbox.x = position_arg.x;
		hitbox.y = position_arg.y;
		positionUpdated();
	}
	void setPosition(phys_t x, phys_t y){
		hitbox.x = x;
		hitbox.y = y;
		positionUpdated();
	}
	box_phys<phys_t> getHitbox() const {return hitbox;}
};

class DynamicEntity: public Entity{
protected:
	vector_phys<phys_t> velocity;
public:
	DynamicEntity(Sprite sprite_arg,
		vector_phys<phys_t> velocity_arg):
		Entity(sprite_arg),velocity(velocity_arg){}

	virtual void update(float delta);

	vector_phys<phys_t> getVelocity() const{ return velocity; }

	bool isFallen();
};


#endif
