#pragma once
#include <iostream>
#include <vector>
#include "Sprite.h"

class UI
{
public:
	bool initialize(std::string title, std::vector<Sprite> gameObjects);
	void update(std::vector<Sprite> gameObjects);
	SDL_Event getInput();
	void cleanup(std::vector<Sprite> gameObjects);

	// Function to load texture from BMP file
	SDL_Texture* loadTexture(std::string path);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	// Function to render the texture
	void render(std::vector<Sprite> gameObjects);
};
