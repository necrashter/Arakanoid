#include "sprite.h"
#include "main.h"

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

Sprite::Sprite(SDL_Texture* t, SDL_Rect region, SDL_Rect pos):
	texture(t), textureRegion(region), position(pos){
}


Sprite::~Sprite() {
	// nothing to free?
}

void Sprite::render(){
	SDL_RenderCopy(renderer, texture, &textureRegion, &position);
}

RenderedText::RenderedText(std::string str,TTF_Font* fnt, SDL_Color color):
	Sprite() ,m_str(str), m_font(fnt), m_color(color)  {
	createTexture();
}

RenderedText::~RenderedText(){
	SDL_DestroyTexture(texture);
}

void RenderedText::setString(const std::string str){
	m_str = str;
	createTexture();
}

void RenderedText::createTexture(){
	SDL_DestroyTexture(texture);
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, m_str.c_str(), m_color);
    if( textSurface == NULL ){
        fprintf(stderr, "Cannot render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }else{
        texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == NULL ){
            fprintf(stderr, "Cannot create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }else{
			textureRegion.w = textSurface->w;
			textureRegion.h = textSurface->h;
            position.w = textSurface->w;
            position.h = textSurface->h;
        }
        SDL_FreeSurface( textSurface );
    }
}
