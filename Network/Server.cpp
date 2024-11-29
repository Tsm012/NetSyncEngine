#pragma once
#include "Server.h"

#include "pch.h"

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

void Server::start() {
  ENetAddress address{};
  address.host = ENET_HOST_ANY;
  address.port = port;
  server = enet_host_create(&address, 1, 2, 0, 0);
  if (server != nullptr) {
    ENetEvent event;
    while (enet_host_service(server, &event, 1000000) > 0) {
      switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
          std::cout << "A new client connected." << std::endl;
          clientPeer = event.peer;
          break;
        case ENET_EVENT_TYPE_RECEIVE: {
          std::vector<unsigned char> receivedData(
              event.packet->data,
              event.packet->data + event.packet->dataLength);
          channel.send(receivedData.data(), event.packet->dataLength);
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
}

bool Server::canFindAPort() {
  ENetAddress address;
  address.host = ENET_HOST_ANY;
  for (port = 2000; port <= 4000; port++) {
    address.port = port;
    ENetHost* server = enet_host_create(&address, 1, 2, 0, 0);
    if (server) {
      enet_host_destroy(server);
      std::cout << "Port selected: " << port << std::endl;
      return true;
    }
  }
  return false;
}
void Server::sendData(const unsigned char* byteArray, size_t size) {
  if (clientPeer != nullptr) {
    ENetPacket* packet =
        enet_packet_create(byteArray, size, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(clientPeer, 0, packet);
    enet_host_flush(server);
  }
}
