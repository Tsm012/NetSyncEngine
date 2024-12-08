#pragma once
#include "pch.h"
#include "Client.h"


namespace Network
{
	Client::Client() : client(nullptr), peer(nullptr)
	{
		if (enet_initialize() != 0)
		{
			std::cerr << "An error occurred while initializing ENet." << std::endl;
		}
	}

	Client::~Client()
	{
		if (client != nullptr)
		{
			enet_host_destroy(client);
		}
		enet_deinitialize();
	}

	void Client::connect(const char* host, uint16_t port)
	{
		client = enet_host_create(nullptr, 1, 2, 0, 0);
		if (client == nullptr)
		{
			std::cerr << "An error occurred while trying to create an ENet client host."
				<< std::endl;
			channel.connected = false;
		}

		ENetAddress address;
		enet_address_set_host(&address, host);
		address.port = port;

		peer = enet_host_connect(client, &address, 2, 0);
		if (peer == nullptr)
		{
			std::cerr << "No available peers for initiating an ENet connection."
				<< std::endl;
			enet_host_destroy(client);
			channel.connected = false;
		}

		ENetEvent event;
		if (enet_host_service(client, &event, 1000000) > 0 &&
			event.type == ENET_EVENT_TYPE_CONNECT)
		{
			std::cout << "Connection to " << host << ":" << port << " succeeded."
				<< std::endl;
			channel.connected = true;
		}
		else
		{
			std::cerr << "Connection to " << host << ":" << port << " failed."
				<< std::endl;
			enet_peer_reset(peer);
			channel.connected = false;
		}

		while (true)
		{
			auto message = channel.fetchSendData();

			if (message.has_value())
			{
				sendData(message.value().data(), message.value().size());
			}
		}
	}

	void Client::sendData(const unsigned char* byteArray, size_t size)
	{
		ENetPacket* packet =
			enet_packet_create(byteArray, size, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		enet_host_flush(client);
	}

	void Client::receiveData()
	{
		ENetEvent event;
		while (enet_host_service(client, &event, 1000000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
			{
				std::vector<unsigned char> receivedData(
					event.packet->data,
					event.packet->data + event.packet->dataLength);
				channel.setReceivedData(receivedData.data(), event.packet->dataLength);
				enet_packet_destroy(event.packet);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
				std::cout << "Disconnected from server." << std::endl;
				return;
			default:
				break;
			}
		}
	}
}
