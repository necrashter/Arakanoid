#include "game_entities.h"
#include <SDL2/SDL.h>
#include <cmath>
#include "main.h"


const float MAX_BAR_SPEED =720.0f;
const float BAR_ACCELERATION=20.0f;

const float BALL_COLLISION_MUL = 0.25f;
const float BALL_MAX_X = 360.0f;

Bar::Bar(Sprite sprite_arg):
	// initial velocity will be zero
	DynamicEntity(sprite_arg,vector_phys<phys_t>(0,0)){

}

void Bar::update(float delta){
	// first change speed depending on the mouse postion
	int mousex,mousey;
	SDL_GetMouseState(&mousex,&mousey);
	//if(mousex>hitbox.w/2.0f && mousex<SCREEN_WIDTH-hitbox.w/2.0f){
		velocity.x = (mousex-(hitbox.x+(hitbox.w/2.0f)));
		velocity.x *= std::fabs(velocity.x)/4.f;
		if(velocity.x>MAX_BAR_SPEED)velocity.x = MAX_BAR_SPEED;
		if(velocity.x<-MAX_BAR_SPEED)velocity.x = -MAX_BAR_SPEED;
	//}
	DynamicEntity::update(delta);
}



Ball::Ball(Sprite sprite_arg,vector_phys<phys_t> speed_arg,Bar* parent):
	DynamicEntity(sprite_arg,speed_arg), bar(parent){

}


void Ball::update(float delta){
	if(bar){
		box_phys<phys_t> pos = bar->getHitbox();
		setPosition(pos.x+pos.w/2.0f-hitbox.w/2.0f,pos.y-hitbox.h);
	}else{
		DynamicEntity::update(delta);
		if(hitbox.x+hitbox.w>SCREEN_WIDTH && velocity.x>0 || velocity.x<0 && hitbox.x<0) velocity.flip_horizontal();
		if(hitbox.y<0) velocity.flip_vertical();
	}
}

void Ball::throwBall(){
	if(bar){
		bar = nullptr;
		velocity.x = 0;
		velocity.y = -180.0f;
	}
}

void Ball::collision(Entity& entity_arg){
	if(checkHorizontalCollision(entity_arg)){
		if(hitbox.x<entity_arg.getHitbox().x && velocity.x>0 ||
			hitbox.x>entity_arg.getHitbox().x && velocity.x<0)
				velocity.flip_horizontal();
	}
	if(checkVerticalCollision(entity_arg)){
		if(hitbox.y<entity_arg.getHitbox().y && velocity.y>0 ||
			hitbox.y>entity_arg.getHitbox().y && velocity.y<0){
				velocity.flip_vertical();// vertical collision
		}
	 }
}

void Ball::barCollision(Bar& barr){
	if(checkVerticalCollision(barr)){
		if(hitbox.y<barr.getHitbox().y && velocity.y>0 ||
				hitbox.y>barr.getHitbox().y && velocity.y<0){
					// flips vertical
			velocity.flip_vertical();
			velocity.x+=BALL_COLLISION_MUL*barr.getVelocity().x;
			if(velocity.x>BALL_MAX_X)velocity.x=BALL_MAX_X;
			else if(velocity.x< -BALL_MAX_X)velocity.x=-BALL_MAX_X;
		}
	}
	if(checkHorizontalCollision(barr)){
		if(hitbox.x<barr.getHitbox().x && velocity.x>0 ||
				hitbox.x>barr.getHitbox().x && velocity.x<0){
			velocity.flip_horizontal();// horizontal collision
			velocity.x+=BALL_COLLISION_MUL*barr.getVelocity().x;
			if(velocity.x>BALL_MAX_X)velocity.x=BALL_MAX_X;
			else if(velocity.x< -BALL_MAX_X)velocity.x=-BALL_MAX_X;
		}
	}
}

Brick::Brick(Sprite sprite_arg, phys_t x, phys_t y):
	Entity(sprite_arg){
	type = none;
	setPosition(x,y);
}
