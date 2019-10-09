#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "load_resources.h"

class Sprite {
private:
	SDL_Texture* texture;
	SDL_Rect textureRegion;
	SDL_Rect position;
public:
	Sprite();
	Sprite(SDL_Texture* texture_arg,
	SDL_Rect textureRegion_arg,
	SDL_Rect position_arg);
	// destructor should not destoy the texture,
	// it will be handled on game exit
	~Sprite();

	void render();

	SDL_Rect getTextureRegion() const { return textureRegion; }
	void setTextureRegion(SDL_Rect& a) { textureRegion = a; }
	 SDL_Rect getPosition() const{ return position; }
	void setPosition(SDL_Rect& a) { position = a; }
	 SDL_Texture* getTexture() const{return texture;}
	void setTexture(SDL_Texture* a) { texture = a; }

	void setPosition(int x,int y){
		position.x=x;
		position.y=y;
	}

	friend class RenderedText;
};

class RenderedText : public Sprite {
private:
	TTF_Font* m_font;
	std::string m_str;
	SDL_Color m_color;
	void createTexture();
public:
	RenderedText(std::string str="", TTF_Font* fnt=regular_font, SDL_Color color=SDL_Color{255,255,255,255});

	~RenderedText();
	// unlike the Sprite, RenderedText should destroy its texture,
	//but not the font

	std::string getString() const { return m_str; }
	void setString(const std::string str); // we need to update the texture
};

#endif
