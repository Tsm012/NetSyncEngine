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
		bool canFindAPort();

	private:
		uint16_t port;
	};
}
