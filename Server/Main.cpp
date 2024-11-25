#pragma once
#include <UI.h>
#include <Server.h>
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

  ui.run(server);
  serverThread.join();

  ui.cleanup();

  return 0;
}