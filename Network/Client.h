#pragma once
#include <enet/enet.h>

#include <iostream>

class Client {
 public:
  Client();
  ~Client();
  bool connect(const char* host, uint16_t port);
  void sendData(const unsigned char* byteArray, size_t size);
  void receiveData();

 private:
  ENetHost* client;
  ENetPeer* peer;
};
