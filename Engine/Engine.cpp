// Engine.cpp : Defines the functions for the static library.
#include "pch.h"
#include "framework.h"
#include "Engine.h"
#include <thread>
#include <map>
#include <Client.h>
#include <Server.h>

Engine::Engine() : connectionType(Server), host("localhost"), port(2000)
{
	initialize(connectionType, host, port);
}
Engine::Engine(ConnectionType connectionType, const char* host, int port) : connectionType(connectionType), host(host), port(port)
{
	initialize(connectionType, host, port);
}

void Engine::initialize(Engine::ConnectionType connectionType, const char* host, int port)
{
	switch (connectionType)
	{
	case Engine::Server:
	{
		connection = new Network::Server();
		break;
	}
	case Engine::Client:
	{
		connection = new Network::Client(host, port);
		break;
	}
	default:
		std::cout << "Invalid Connection Type";
		return;
	}

	if (!ui.initialize(connectionType == Server ? "Server" : "Client"))
	{
		std::cout << "Could not initialize UI" << std::endl;
	}
}
void Engine::run()
{
	std::thread sendThread(&NetworkConnection::start, connection);

	while (!connection->connected && !connection->hasError)
	{
		Sleep(100);
	}

	std::thread receiveThread(&NetworkConnection::receiveData, connection);

	while (ui.running && connection->connected && !connection->hasError)
	{
		update();
	}

	sendThread.join();
	receiveThread.join();
	ui.cleanup();
}

void Engine::update()
{
	std::optional<std::vector<unsigned char>> message =
		connection->getChannel().fetchReceivedData();

	if (message.has_value())
	{
		if (message.value().size() == sizeof(ui.gameObjects))
		{
			Object gameObjects[sizeof(ui.gameObjects)];
			std::memcpy(&gameObjects, message.value().data(), sizeof(ui.gameObjects));
			for (size_t i = 0; i < std::size(ui.gameObjects); ++i)
			{
				ui.gameObjects[i].boundingBox = gameObjects[i].boundingBox;
			}
		}
	}
	ui.update();

	if (ui.changed)
	{
		unsigned char byteArray[sizeof(ui.gameObjects)];
		std::memcpy(byteArray, &ui.gameObjects, sizeof(ui.gameObjects));
		connection->getChannel().setDataToSend(byteArray, sizeof(ui.gameObjects));
		ui.changed = false;
	}
}
