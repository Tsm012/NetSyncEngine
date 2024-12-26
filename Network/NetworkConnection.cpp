
#pragma once
#include "pch.h"
#include "NetworkConnection.h"
#include <iostream>
#include <ThreadSafeByteChannel.h>
#include "Message.h"


void NetworkConnection::sendData(ENetPeer* peer, Network::Message message)
{
	if (peer != nullptr)
	{
		ENetPacket* packet =
			enet_packet_create(message.serializeMessage().data(), message.serializeMessage().size(), ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
	}
	enet_host_flush(host);
}

NetworkConnection::NetworkConnection() : host(nullptr), channel()
{
}


void NetworkConnection::receiveData()
{
	ENetEvent event;
	while (enet_host_service(host, &event, 1000000) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
		{
			std::cout << "A new connection is established." << std::endl;
			connected = true;
			getChannel().peers.push_back(event.peer);
			getChannel().addMessageChannel(event.peer->incomingPeerID);
			break;
		}
		case ENET_EVENT_TYPE_RECEIVE:
		{
			auto message = Network::Message(event.packet->data, event.packet->dataLength);
			getChannel().setReceivedData(event.peer->incomingPeerID, message);
			std::cout << "Received data of type: " << message.getMessageType() << std::endl;
			enet_packet_destroy(event.packet);
			break;
		}
		case ENET_EVENT_TYPE_DISCONNECT:
			std::cout << "Client disconnected." << std::endl;
			getChannel().peers.erase(std::remove(getChannel().peers.begin(), getChannel().peers.end(), event.peer), getChannel().peers.end());
			getChannel().removeMessageChannel(event.peer->incomingPeerID);
			break;
		default:
			break;
		}
	}
}

