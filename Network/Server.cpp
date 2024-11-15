#pragma once
#include "pch.h"
#include "Server.h"

Server::Server() : server(nullptr), clientPeer(nullptr) {
  if (enet_initialize() != 0) {
    std::cerr << "An error occurred while initializing ENet." << std::endl;
  }
}

Server::~Server() {
  if (server != nullptr) {
    enet_host_destroy(server);
  }
  enet_deinitialize();
}

bool Server::start(uint16_t port) {
  ENetAddress address{};
  address.host = ENET_HOST_ANY;
  address.port = port;
  server = enet_host_create(&address, 1, 2, 0, 0);
  return server != nullptr;
}

void Server::receiveData() {
  ENetEvent event;
  while (enet_host_service(server, &event, 1000000) > 0) {
    switch (event.type) {
    case ENET_EVENT_TYPE_CONNECT:
      std::cout << "A new client connected." << std::endl;
      clientPeer = event.peer;
      break;
    case ENET_EVENT_TYPE_RECEIVE: {
      std::string receivedData(reinterpret_cast<char *>(event.packet->data),
                               event.packet->dataLength);
      channel.send(receivedData);
      enet_packet_destroy(event.packet);
      break;
    }
    case ENET_EVENT_TYPE_DISCONNECT:
      std::cout << "Client disconnected." << std::endl;
      clientPeer = nullptr;
      break;
    default:
      break;
    }
  }
}

void Server::sendData(const char *message) {
  if (clientPeer != nullptr) {
    ENetPacket *packet = enet_packet_create(message, strlen(message) + 1,
                                            ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(clientPeer, 0, packet);
    enet_host_flush(server);
  }
}
