#pragma once
#include <SDL3/SDL.h>
class Sprite
{
public:
	SDL_Texture* texture;
	SDL_FRect boundingBox;
	int moveStep;
	Sprite();
	Sprite(SDL_Texture* texture, const SDL_FRect& boundingBox, int moveStep);
private:
};


