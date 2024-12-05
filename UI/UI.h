#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <Client.h>
#include <Server.h>

class UI
{
public:
	bool initialize(std::string title);
	void update();
	void getInput();
	void cleanup();

	SDL_FRect redBox = { 100, 150, 100, 100 };  // Fixed size for red box

	bool running = true;
	bool changed = true;

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Event event;

	int moveStep = 15;

	// Function to load texture from BMP file
	bool loadTexture(std::string path);

	// Function to render the texture
	void render();
};
