#pragma once
#include "Client.h"

#include "pch.h"

Client::Client() : client(nullptr), peer(nullptr) {
  if (enet_initialize() != 0) {
    std::cerr << "An error occurred while initializing ENet." << std::endl;
  }
}

Client::~Client() {
  if (client != nullptr) {
    enet_host_destroy(client);
  }
  enet_deinitialize();
}

bool Client::connect(const char *host, uint16_t port) {
  client = enet_host_create(nullptr, 1, 2, 0, 0);
  if (client == nullptr) {
    std::cerr << "An error occurred while trying to create an ENet client host."
              << std::endl;
    return false;
  }

  ENetAddress address;
  enet_address_set_host(&address, host);
  address.port = port;

  peer = enet_host_connect(client, &address, 2, 0);
  if (peer == nullptr) {
    std::cerr << "No available peers for initiating an ENet connection."
              << std::endl;
    enet_host_destroy(client);
    return false;
  }

  ENetEvent event;
  if (enet_host_service(client, &event, 1000000) > 0 &&
      event.type == ENET_EVENT_TYPE_CONNECT) {
    std::cout << "Connection to " << host << ":" << port << " succeeded."
              << std::endl;
    return true;
  } else {
    std::cerr << "Connection to " << host << ":" << port << " failed."
              << std::endl;
    enet_peer_reset(peer);
    return false;
  }
}

void Client::sendData(const unsigned char *byteArray, size_t size) {
  ENetPacket *packet =
      enet_packet_create(byteArray, size, ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(peer, 0, packet);
  enet_host_flush(client);
}

void Client::receiveData() {
  ENetEvent event;
  while (enet_host_service(client, &event, 1000) > 0) {
    switch (event.type) {
      case ENET_EVENT_TYPE_RECEIVE:
        std::cout << "A packet of length " << event.packet->dataLength
                  << " containing '" << event.packet->data << "' was received."
                  << std::endl;
        enet_packet_destroy(event.packet);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        std::cout << "Disconnected from server." << std::endl;
        return;
      default:
        break;
    }
  }
}
