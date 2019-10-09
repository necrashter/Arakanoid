#include "entity.h"

Entity::Entity(Sprite sprite_arg):
	sprite_entity(sprite_arg){
		SDL_Rect rect = sprite_entity.getPosition();
		hitbox.x= rect.x;
		hitbox.y= rect.y;
		hitbox.w= rect.w;
		hitbox.h= rect.h;
}

void Entity::setSprite_entity(SDL_Texture* texture_arg,
SDL_Rect textureRegion_arg,
SDL_Rect position_arg){
	sprite_entity.setTexture(texture_arg);
	sprite_entity.setTextureRegion(textureRegion_arg);
	sprite_entity.setPosition(position_arg);
}

bool Entity::checkCollision(const Entity& other){
	return hitbox.intersects(other.getHitbox());
}

void Entity::render(){
	sprite_entity.render();
}
