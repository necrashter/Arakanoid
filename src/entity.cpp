#include "entity.h"
#include "math.h"
#include "main.h"

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

void Entity::setTextureRegion(SDL_Rect reg){
	sprite_entity.setTextureRegion(reg);
}

bool Entity::checkCollision(const Entity& other){
	return hitbox.intersects(other.getHitbox());
}

bool Entity::checkHorizontalCollision(const Entity& other){
	return hitbox.intersectsHorizontal(other.getHitbox());
}
bool Entity::checkVerticalCollision(const Entity& other){
	return hitbox.intersectsVertical(other.getHitbox());
}

void Entity::render(){
	sprite_entity.render();
}

void Entity::positionUpdated(){
	sprite_entity.setPosition(roundf(hitbox.x),roundf(hitbox.y));
}


/////////////// DynamicEntity //////////////

void DynamicEntity::update(float delta) {
	hitbox += velocity*delta;
	positionUpdated();
}

bool DynamicEntity::isFallen(){
	return hitbox.y>SCREEN_HEIGHT;
}
