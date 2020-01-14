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
private:
	int sizeLevel = 0;
	int bulletNumber=0;
	Sprite leftGun,rightGun;
	bool hasGuns;
	void positionUpdated();
public:
	Bar(Sprite sprite_arg,Sprite gunSprite);
	void update(float delta);
	void changeSize(int new_size, SDL_Rect& new_reg);
	int getSizeLevel() const;
	void arm();
	bool getHasGuns() const;
	vector_phys<phys_t> getLeftBulletPos() const;
	vector_phys<phys_t> getRightBulletPos() const;
	void render();
	int getBulletNumber();
	void setBulletNumber(int x);
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

class Bullet: public DynamicEntity{
public:
	Bullet(Sprite sprite_arg,vector_phys<phys_t> speed_arg):DynamicEntity(sprite_arg,speed_arg){}
};


#endif
