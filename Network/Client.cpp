#pragma once
#include "pch.h"
#include "Client.h"
#include <random>
#include <thread>


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

	unsigned int Client::handShake(Network::Message message2)
	{
		std::cout << "Sending message of type: " << message2.getMessageType() << std::endl;
		sendData(peer, message2);
		auto channels = channel.fetchReceivedData();

		for (auto& channel : channels)
		{
			for (auto& message : channel.second)
			{
				std::cout << "Message value[0] is not nullptr." << std::endl;
				if (message.second.getMessageType() == Network::Message::Hello)
				{
					std::cout << "Message type is Hello." << std::endl;
					unsigned int thing = 0;
					std::memcpy(&thing, message.second.getPayload().data(), sizeof(unsigned int));
					return thing;
				}
			}
		}

		return 0;
	}

	void Client::start()
	{
		ENetEvent event;
		while (enet_host_service(host, &event, 10000) > 0 &&
			event.type == ENET_EVENT_TYPE_CONNECT)
		{
			std::cout << "Connection to " << peer->address.host << ":" << peer->address.port << " succeeded."
				<< std::endl;
			connected = true;
		}
		/*else
		{
		std::cerr << "Connection to " << peer->address.host << ":" << peer->address.port << " failed."
		<< std::endl;
		enet_peer_reset(peer);
		hasError = true;
		return;
		}*/

		getChannel().addMessageChannel(peer->outgoingPeerID);

		while (true)
		{
			auto messages = channel.fetchSendData();

			for (auto& message : messages)
			{
				std::cout << "Sending message of type: " << message.second.getMessageType() << std::endl;
				sendData(peer, message.second);
				channel.removeSendData(message.first);
			}

		}
	}

	unsigned int Client::generateRandomId()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(100000, 999999);
		return dis(gen);
	}
}
