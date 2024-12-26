#pragma once
#include <ThreadSafeByteChannel.h>
#include "NetworkConnection.h"
#include <iostream>

namespace Network
{
	class Client : public NetworkConnection
	{
	public:
		Client(const char* hostAddress, int port);
		~Client();

		enum ConnectionState
		{
			Initializing,
			Connected
		};


		unsigned int generateRandomId();
		void start() override;
		void sendHello(Network::Message message) override;
		unsigned int getGreeting() override;
		ENetPeer* peer;
	private:
		ConnectionState state = ConnectionState::Initializing;
	};
}
