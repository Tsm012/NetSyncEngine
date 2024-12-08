#include "Object.h"
Object::Object() : texture(nullptr), boundingBox(SDL_FRect{ 100,100,100,100 }), moveStep(15)
{
}
Object::Object(SDL_Texture* texture, const SDL_FRect& boundingBox, int moveStep)
	: texture(texture), boundingBox(boundingBox), moveStep(moveStep)
{
}
