#pragma once
#include <ThreadSafeByteChannel.h>
#include <enet/enet.h>

#include <iostream>

class Client
{
public:
	Client();
	~Client();
	void connect(const char* host, uint16_t port);
	void sendData(const unsigned char* byteArray, size_t size);
	void receiveData();

	ThreadSafeByteChannel& getChannel()
	{
		return channel;
	}

private:
	ENetHost* client;
	ENetPeer* peer;
	ThreadSafeByteChannel channel;
};
