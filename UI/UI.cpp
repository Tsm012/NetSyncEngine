#include "UI.h"
#include "Object.h"
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

	//TODO remove once we have a way to save state
	float offset = 100;
	for (Object& object : gameObjects)
	{
		object.boundingBox = SDL_FRect{ offset, 100, 175, 100 };
		object.texture = loadTexture("Player.bmp");
		object.moveStep = 15;
		offset = offset + 200;
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

void UI::update()
{
	getInput();

	render();
}

//TODO seperate the game object updates from this function
void UI::getInput()
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			running = false;
		}
		else if (event.type == SDL_EVENT_KEY_DOWN)
		{
			for (Object& object : gameObjects)
			{
				switch (event.key.scancode)
				{
				case SDL_SCANCODE_UP:
					object.boundingBox.y -= object.moveStep;
					changed = true;
					break;
				case SDL_SCANCODE_DOWN:
					object.boundingBox.y += object.moveStep;
					changed = true;
					break;
				case SDL_SCANCODE_LEFT:
					object.boundingBox.x -= object.moveStep;
					changed = true;
					break;
				case SDL_SCANCODE_RIGHT:
					object.boundingBox.x += object.moveStep;
					changed = true;
					break;
				}
			}
		}
	}
}

void UI::render()
{
	// Clear the window
	SDL_RenderClear(renderer);
	for (Object object : gameObjects)
	{
		SDL_RenderTexture(renderer, object.texture, nullptr, &object.boundingBox);  // Use SDL_RenderCopyF
	}


	// Update the screen
	SDL_RenderPresent(renderer);
}

void UI::cleanup()
{
	for (Object object : gameObjects)
	{
		if (object.texture != nullptr)
		{
			SDL_DestroyTexture(object.texture);
			//object.texture = nullptr;
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}


