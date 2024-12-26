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


		unsigned int generateRandomId();
		void start() override;
		unsigned int handShake(Network::Message message) override;
		ENetPeer* peer;
	};
}
