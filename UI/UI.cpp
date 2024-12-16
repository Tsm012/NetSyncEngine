#include "UI.h"
#include "Sprite.h"
#include <SDL3_image/SDL_image.h> // Include SDL_image


bool UI::initialize(std::string title, std::vector<Sprite> gameObjects)
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

	// Set render draw color (white)
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	return true;
}

SDL_Texture* UI::loadTexture(std::string path)
{
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == nullptr)
	{
		std::cerr << "Unable to load image " << path << "! IMG_Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	// Check if the surface is valid
	if (loadedSurface->w == 0 || loadedSurface->h == 0)
	{
		std::cerr << "Invalid surface dimensions!" << std::endl;
		SDL_DestroySurface(loadedSurface);
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	SDL_DestroySurface(loadedSurface);

	if (texture == nullptr)
	{
		std::cerr << "Unable to create texture from " << path << "! SDL_Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	return texture;
}

void UI::update(std::vector<Sprite> gameObjects)
{
	getInput();

	render(gameObjects);
}

SDL_Event UI::getInput()
{
	while (SDL_PollEvent(&event))
	{
		return event;

	}
}

void UI::render(std::vector<Sprite> gameObjects)
{
	// Clear the window
	SDL_RenderClear(renderer);
	for (Sprite object : gameObjects)
	{
		SDL_RenderTexture(renderer, object.texture, nullptr, &object.boundingBox);
	}

	// Update the screen
	SDL_RenderPresent(renderer);
}

void UI::cleanup(std::vector<Sprite> gameObjects)
{
	for (Sprite object : gameObjects)
	{
		if (object.texture != nullptr)
		{
			SDL_DestroyTexture(object.texture);
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}


