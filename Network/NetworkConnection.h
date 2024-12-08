#pragma once
#include <enet/enet.h>
#include <ThreadSafeByteChannel.h>

class NetworkConnection
{
public:
	virtual void start() = 0;
	void receiveData();

	ThreadSafeByteChannel& getChannel()
	{
		return channel;
	}
	bool connected = false;
	bool hasError = false;
protected:
	ENetHost* host;
	ENetPeer* peer;
	ThreadSafeByteChannel channel;
	void sendData(const unsigned char* byteArray, size_t size);
};


