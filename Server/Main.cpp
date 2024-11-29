#pragma once
#include <Server.h>
#include <UI.h>

#include <thread>

int main(int argc, char *args[]) {
  UI ui;

  if (!ui.initialize("Server")) {
    return 1;
  }

  Server server;

  if (!server.canFindAPort()) {
    return 0;
  }

  std::thread serverThread(&Server::start, &server);

  while (ui.running) {
    std::optional<std::vector<unsigned char>> message =
        server.getChannel().receive();
    if (message.has_value()) {
      if (message.value().size() == sizeof(SDL_Rect)) {
        SDL_Rect rect;
        std::memcpy(&rect, message.value().data(), sizeof(SDL_Rect));
        ui.redBox = rect;
      }
    }
    ui.update();
  }

  serverThread.join();

  ui.cleanup();

  return 0;
}