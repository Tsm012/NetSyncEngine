#pragma once
#include "pch.h"
#include "Client.h"


namespace Network
{
	Client::Client(const char* hostAddress, int port)
	{
		if (enet_initialize() != 0)
		{
			std::cerr << "An error occurred while initializing ENet." << std::endl;
		}

		host = enet_host_create(nullptr, 1, 2, 0, 0);
		if (host == nullptr)
		{
			std::cerr << "An error occurred while trying to create an ENet client host."
				<< std::endl;
		}

		ENetAddress address;
		enet_address_set_host(&address, hostAddress);
		address.port = port;
		peer = enet_host_connect(host, &address, 2, 0);

		if (peer == nullptr)
		{
			std::cerr << "No available peers for initiating an ENet connection."
				<< std::endl;
			enet_host_destroy(host);
		}
	}

	Client::~Client()
	{
		if (host != nullptr)
		{
			enet_host_destroy(host);
		}
		enet_deinitialize();
	}

	void Client::start()
	{
		ENetEvent event;
		if (enet_host_service(host, &event, 10000) > 0 &&
			event.type == ENET_EVENT_TYPE_CONNECT)
		{
			std::cout << "Connection to " << peer->address.host << ":" << peer->address.port << " succeeded."
				<< std::endl;
			connected = true;
		}
		else
		{
			std::cerr << "Connection to " << peer->address.host << ":" << peer->address.port << " failed."
				<< std::endl;
			enet_peer_reset(peer);
			hasError = true;
			return;
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


}
