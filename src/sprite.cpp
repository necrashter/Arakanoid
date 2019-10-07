#include "sprite.h"

Sprite::Sprite(): texture(NULL) {
	textureRegion.x = 0;
	textureRegion.y = 0;
	textureRegion.w = 0;
	textureRegion.h = 0;
	position.x = 0;
	position.y = 0;
	position.w = 0;
	position.h = 0;
}

Sprite::~Sprite() {
	// nothing to free?
}
