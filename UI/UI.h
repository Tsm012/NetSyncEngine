#pragma once
#include <Client.h>
#include <SDL.h>
#include <Server.h>
#include <iostream>
#include <optional>

class UI {
public:
  bool initialize(std::string title);
  SDL_Rect update();
  void cleanup();
  bool running = true;
  SDL_Rect redBox = {100, 150, 100, 100}; // Fixed size for red box
  bool changed = true;

private:
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Event event;
  int moveStep = 10;
};
