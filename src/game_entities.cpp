#include "game_entities.h"
#include <SDL2/SDL.h>
#include <cmath>
#include "main.h"
const float MAX_BAR_SPEED =360.0f;
const float BAR_ACCELERATION=20.0f;

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



Ball::Ball(Sprite sprite_arg,vector_phys<phys_t> speed_arg):
	DynamicEntity(sprite_arg,speed_arg){

}


void Ball::update(float delta){
	DynamicEntity::update(delta);
}

void Ball::collision(Entity& entity_arg){
	if(checkHorizontalCollision(entity_arg)) velocity.flip_horizontal();
	else velocity.flip_vertical();// vertical collision
}

void Ball::barCollision(Bar& barr){
	if(checkVerticalCollision(barr)){
		velocity.flip_vertical();
		velocity.x+=barr.getVelocity().x;
	}
	else{
		velocity.flip_horizontal();// horizontal collision
		//is velocity.x+=((Bar*)entity_arg)->get_x(); needed or not?
	}
}

Brick::Brick(Sprite sprite_arg, phys_t x, phys_t y):
	Entity(sprite_arg){
	setPosition(x,y);
}
