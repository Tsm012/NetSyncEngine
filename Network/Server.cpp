#pragma once
#include "pch.h"
#include "Server.h"
#include <iostream>

namespace Network
{
	Server::Server()
	{
		if (enet_initialize() != 0)
		{
			std::cerr << "An error occurred while initializing ENet." << std::endl;
		}

		if (!canFindAPort())
		{
			std::cerr << "Unable to find open port between 2000 - 4000"
				<< std::endl;
			hasError = true;
		}

		ENetAddress address{};
		address.host = ENET_HOST_ANY;
		address.port = port;
		host = enet_host_create(&address, 1, 2, 0, 0);

		if (host == nullptr)
		{
			std::cerr << "An error occurred while trying to create an ENet client host."
				<< std::endl;
		}
	}

	Server::~Server()
	{
		if (host != nullptr)
		{
			enet_host_destroy(host);
		}
		enet_deinitialize();
	}

	void Server::start()
	{
		connected = true;
		while (true)
		{
			auto message = channel.fetchSendData();

			if (message.has_value())
			{
				sendData(message.value().data(), message.value().size());
			}
		}
	}

	bool Server::canFindAPort()
	{
		ENetAddress address{};
		address.host = ENET_HOST_ANY;
		for (port = 2000; port <= 4000; port++)
		{
			address.port = port;
			ENetHost* server = enet_host_create(&address, 1, 2, 0, 0);
			if (server)
			{
				enet_host_destroy(server);
				std::cout << "Server port selected: " << port << std::endl;
				return true;
			}
		}
		return false;
	}
}
