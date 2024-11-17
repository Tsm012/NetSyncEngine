#pragma once
#include <enet/enet.h>
#include <iostream>
#include "ThreadSafeStringChannel.h"

class Server {
public:
    Server();
    ~Server();
    bool canFindAPort(); 
    void start();
    void sendData(const char* message);

    ThreadSafeStringChannel& getChannel() { return channel; }

private:
    ENetHost* server;
    ENetPeer* clientPeer;
    ThreadSafeStringChannel channel;
    uint16_t port;
};

