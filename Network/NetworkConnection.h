#pragma once
#include <enet/enet.h>
#include <ThreadSafeByteChannel.h>
#include <Message.h>

class NetworkConnection
{
public:
	NetworkConnection();
	virtual void start() = 0;
	virtual void sendHello(Network::Message message) = 0;
	virtual unsigned int getGreeting() = 0;
	void receiveData();

	ThreadSafeByteChannel& getChannel()
	{
		return channel;
	}
	bool connected = false;
protected:
	ENetHost* host;
	ThreadSafeByteChannel channel;
	void sendData(ENetPeer* peer, Network::Message message);

};


