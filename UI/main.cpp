#include <Client.h>
#include <Server.h>
#include <thread>
#include <optional>
#include "UI.h"

#define SERVER

void runServer(Server& server) {
    if (server.start(1234)) {
        while (true) {
            server.receiveData();
        }
    }
}

int main(int argc, char* args[]) {
    bool running = true;
    UI ui;

#ifdef SERVER
    Server server;
    std::thread serverThread(runServer, std::ref(server));

    if (!ui.initialize()) {
        return 1;
    }

    ui.handleEvents(server, running);
    ui.cleanup();

    serverThread.join();
#else
    Client client;
    if (client.connect("localhost", 1234)) {
        if (!ui.initialize()) {
            return 1;
        }

        ui.handleEvents(client, running);
        ui.cleanup();
    }
#endif

    return 0;
}
