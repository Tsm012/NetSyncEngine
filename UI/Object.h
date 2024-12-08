#pragma once
#include <SDL3/SDL.h>
class Object
{
public:
	SDL_Texture* texture;
	SDL_FRect boundingBox;
	int moveStep;
	Object();
	Object(SDL_Texture* texture, const SDL_FRect& boundingBox, int moveStep);
private:
};


