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


AnimatedSprite::AnimatedSprite(SDL_Texture* texture_arg,SDL_Rect* frames_arg,int len, int x, int y):
	Sprite(texture_arg, frames_arg[0], SDL_Rect{x,y,frames_arg[0].w,frames_arg[0].h}),
	 frames(frames_arg), length(len){
		current_frame=0;
		speed = 45.f;
		timeSinceLastFrame=  0;
	}

bool AnimatedSprite::update(float delta){
	timeSinceLastFrame+=delta;
	if(timeSinceLastFrame>=1.0f/speed){
		if(current_frame==length-1)return true;
		timeSinceLastFrame-=1.0f/speed;
		textureRegion=frames[++current_frame];
	}
	return false;
}
