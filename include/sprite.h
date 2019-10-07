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
	Sprite();

	// destructor should not destoy the texture,
	// it will be handled on game exit
	~Sprite();

	// TODO: Write getter and setters for all private members
	// getters and setters can be defined right here,
	// as we want these functions to be inlined
	// don't forget to make getters const

	//const SDL_Rect& getTextureRegion() const { return textureRegion; }
	SDL_Rect getTextureRegion() { return textureRegion; }
	void setTextureRegion(SDL_Rect& a) { textureRegion = a; }
};

#endif
