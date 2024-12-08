#pragma once
#include <iostream>
#include "Object.h"

class UI
{
public:
	bool initialize(std::string title);
	void update();
	void getInput();
	void cleanup();


	bool running = true;
	bool changed = true;
	Object gameObjects[3];

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;



	// Function to load texture from BMP file
	SDL_Texture* loadTexture(std::string path);

	// Function to render the texture
	void render();
};
