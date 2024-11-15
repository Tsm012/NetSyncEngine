#pragma once
#include <enet/enet.h>
#include <iostream>
#include "ThreadSafeStringChannel.h"

class Server {
public:
    Server();
    ~Server();
    bool start(uint16_t port);
    void receiveData();
    void sendData(const char* message);
    ThreadSafeStringChannel& getChannel() { return channel; }

private:
    ENetHost* server;
    ENetPeer* clientPeer;
    ThreadSafeStringChannel channel;
};

