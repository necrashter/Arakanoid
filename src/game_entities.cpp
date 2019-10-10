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
