#pragma once
#include <enet/enet.h>
#include <iostream>

class Client {
public:
    Client();
    ~Client();
    bool connect(const char* host, uint16_t port);
    void sendData(const char* message);
    void receiveData();


private:
    ENetHost* client;
    ENetPeer* peer;
};
