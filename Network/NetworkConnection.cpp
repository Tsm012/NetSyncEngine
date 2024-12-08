
#pragma once
#include "pch.h"
#include "NetworkConnection.h"
#include <iostream>
#include <ThreadSafeByteChannel.h>

void NetworkConnection::sendData(const unsigned char* byteArray, size_t size)
{
	if (peer != nullptr)
	{
		ENetPacket* packet =
			enet_packet_create(byteArray, size, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
	}
	enet_host_flush(host);
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
			const char* ack = "";
			sendData(reinterpret_cast<const unsigned char*>(ack), strlen(ack) + 1);
			connected = true;
			peer = event.peer;
			break;
		}
		case ENET_EVENT_TYPE_RECEIVE:
		{
			std::vector<unsigned char> receivedData(
				event.packet->data,
				event.packet->data + event.packet->dataLength);
			std::cout << "received data" << std::endl;
			getChannel().setReceivedData(receivedData.data(), event.packet->dataLength);
			enet_packet_destroy(event.packet);
			break;
		}
		case ENET_EVENT_TYPE_DISCONNECT:
			std::cout << "Client disconnected." << std::endl;
			peer = nullptr;
			break;
		default:
			break;
		}
	}
}
