#pragma once
#include <enet/enet.h>
#include "NetworkConnection.h"


namespace Network
{
	class Server : public NetworkConnection
	{
	public:
		Server();
		~Server();
		void start() override;
		void sendHello(Network::Message message) override;
		unsigned int getGreeting() override;
		bool canFindAPort();

	private:
		uint16_t port = 2000;
	};
}
