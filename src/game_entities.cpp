#include "game_entities.h"
#include <SDL2/SDL.h>
#include <cmath>

Bar::Bar(Sprite sprite_arg):
	// initial velocity will be zero
	DynamicEntity(sprite_arg,vector_phys<phys_t>(0,0)){

}

void Bar::update(float delta){
	// first change speed depending on the mouse postion
	int mousex,mousey;
	SDL_GetMouseState(&mousex,&mousey);
	velocity.x = (mousex-hitbox.x)*100.0;
	if(velocity.x>MAX_BAR_SPEED)velocity.x = MAX_BAR_SPEED;
	if(velocity.x<-MAX_BAR_SPEED)velocity.x = -MAX_BAR_SPEED;
	DynamicEntity::update(delta);
}

void Ball::collision(Entity* entity_arg){
	string entityType= entity_arg->get_entityType();
	if(entityType=="Brick"){
		if(1/*according to collision,horizontal*/) velocity.flip_horizontal();
		else velocity.flip_vertical();// vertical collision
		((Brick*)entity_arg)->breaking();
	}
	else if(entityType=="Bar"){
			if(1/*according to collision,vertical*/){
				velocity.flip_vertical();
				velocity.x+=((Bar*)entity_arg)->get_x();
			}
			else{
				velocity.flip_horizontal();// horizontal collision
				//is velocity.x+=((Bar*)entity_arg)->get_x(); needed or not?
			}

	}
}
