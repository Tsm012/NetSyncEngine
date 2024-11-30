#pragma once
#include <ThreadSafeByteChannel.h>
#include <enet/enet.h>

#include <iostream>

class Server
{
public:
	Server();
	~Server();
	bool canFindAPort();
	void start();
	void sendData(const unsigned char* byteArray, size_t size);

	ThreadSafeByteChannel& getChannel()
	{
		return channel;
	}

private:
	ENetHost* server;
	ENetPeer* clientPeer;
	ThreadSafeByteChannel channel;
	uint16_t port;
};
