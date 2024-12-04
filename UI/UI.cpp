#include "UI.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h> // Include SDL_image


bool UI::initialize(std::string title)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow(title.c_str(), 800, 600, 0);
	if (window == nullptr)
	{
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	renderer = SDL_CreateRenderer(window, nullptr);
	if (renderer == nullptr)
	{
		std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	// Initialize SDL_image
	if (!IMG_Init(IMG_INIT_PNG))
	{
		std::cerr << "SDL_image could not initialize! IMG_Error: " << SDL_GetError() << std::endl;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	// Load the player image
	if (!loadTexture(renderer, "Player.bmp"))
	{
		std::cerr << "Failed to load texture!" << std::endl;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		return false;
	}

	return true;
}


bool UI::loadTexture(SDL_Renderer* renderer, std::string path)
{
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == nullptr)
	{
		std::cerr << "Unable to load image " << path << "! IMG_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Check if the surface is valid
	if (loadedSurface->w == 0 || loadedSurface->h == 0)
	{
		std::cerr << "Invalid surface dimensions!" << std::endl;
		SDL_DestroySurface(loadedSurface);
		return false;
	}

	// Print surface details
	std::cout << "Surface loaded: " << path << "\n"
		<< "Width: " << loadedSurface->w << ", Height: " << loadedSurface->h << std::endl;

	texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	SDL_DestroySurface(loadedSurface);

	if (texture == nullptr)
	{
		std::cerr << "Unable to create texture from " << path << "! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

SDL_FRect UI::update()
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			running = false;
		}
		else if (event.type == SDL_EVENT_KEY_DOWN)
		{
			switch (event.key.scancode)
			{
			case SDL_SCANCODE_UP:
				redBox.y -= moveStep;
				changed = true;
				break;
			case SDL_SCANCODE_DOWN:
				redBox.y += moveStep;
				changed = true;
				break;
			case SDL_SCANCODE_LEFT:
				redBox.x -= moveStep;
				changed = true;
				break;
			case SDL_SCANCODE_RIGHT:
				redBox.x += moveStep;
				changed = true;
				break;
			}
		}
	}

	// Set render draw color (white)
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Clear the window
	SDL_RenderClear(renderer);

	// Render the texture
	renderTexture(redBox);

	// Set render draw color (red) for the red box

	// Update the screen
	SDL_RenderPresent(renderer);

	// Delay to control frame rate (e.g., 60 FPS)
	SDL_Delay(16);
	return redBox;
}

void UI::renderTexture(SDL_FRect redbox)
{
	SDL_FRect destRect = { 400.0f, 300.0f, 100.0f, 100.0f }; // Position and size of the image
	SDL_RenderTexture(renderer, texture, nullptr, &redbox);  // Use SDL_RenderCopyF
}

void UI::cleanup()
{
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();  // Quit SDL_image
	SDL_Quit();
}
