#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <Client.h>
#include <Server.h>

class UI
{
public:
	bool initialize(std::string title);
	SDL_FRect update();
	void cleanup();
	bool running = true;
	SDL_FRect redBox = { 100, 150, 100, 100 };  // Fixed size for red box
	bool changed = true;

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;  // Texture for the player image
	SDL_Event event;
	int moveStep = 10;

	// Function to load texture from BMP file
	bool loadTexture(SDL_Renderer* renderer, std::string path);

	// Function to render the texture
	void renderTexture(SDL_FRect redbox);
};
