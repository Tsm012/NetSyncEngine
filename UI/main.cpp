#include "main.h"
#include "UI.h"
#include <Client.h>
#include <Server.h>
#include <optional>
#include <thread>

int main(int argc, char *args[]) {
  UI ui;

  Mode mode = SelectNetworkMode();

  if (!ui.initialize()) {
    return 1;
  }

  if (mode == SERVER_MODE) {
    Server server;
    
    if (!server.canFindAPort()) {
      return 0;
    }

    std::thread serverThread(&Server::start, &server);

    ui.run(server);
    serverThread.join();
  } else if(mode == CLIENT_MODE) {
    int port;
    std::cout << "Enter port to connect to: ";
    std::cin >> port;
    Client client;
    if (client.connect("localhost", port)) {
      ui.run(client);
    }
  }

  ui.cleanup();

  return 0;
}

Mode SelectNetworkMode() {
  int selection = 0;
  while (selection != SERVER_MODE && selection != CLIENT_MODE) {
    std::cout << "Select mode:\n";
    std::cout << "1. Server\n";
    std::cout << "2. Client\n";
    std::cout << "Enter 1 or 2: ";
    std::cin >> selection;
    if (selection != SERVER_MODE && selection != CLIENT_MODE) {
      std::cout << "Invalid selection. Please enter 1 or 2.\n";
    }
  }
  return static_cast<Mode>(selection);
}
