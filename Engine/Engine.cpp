// Engine.cpp : Defines the functions for the static library.
#include "pch.h"
#include "framework.h"
#include "Engine.h"
#include <thread>
#include <map>
#include <Client.h>
#include <UI.h>
#include <Server.h>

Engine::Engine() : connectionType(Server), host("localhost"), port(2000)
{
}

Engine::Engine(ConnectionType connectionType, const char* host, int port) : connectionType(connectionType), host(std::move(host)), port(port)
{
}

void Engine::run()
{
	UI ui;

	if (!ui.initialize(connectionType == Server ? "Server" : "Client"))
	{
		return;
	}

	switch (connectionType)
	{
	case Engine::Server:
	{
		Network::Server server;
		if (!server.canFindAPort())
		{
			return;
		}

		std::thread serverThread(&Network::Server::start, &server);
		while (ui.running)
		{
			std::optional<std::vector<unsigned char>> message =
				server.getChannel().fetchReceivedData();

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
		}

		serverThread.join();
		break;
	}
	case Engine::Client:
	{
		Network::Client client;
		std::thread clientSendThread(&Network::Client::connect, &client, host, port);
		while (!client.getChannel().connected)
		{
			Sleep(100);
		}
		std::thread clientReceiveThread(&Network::Client::receiveData, &client);
		while (ui.running)
		{
			ui.update();

			if (ui.changed)
			{
				unsigned char byteArray[sizeof(ui.gameObjects)];
				std::memcpy(byteArray, &ui.gameObjects, sizeof(ui.gameObjects));
				client.getChannel().setDataToSend(byteArray, sizeof(ui.gameObjects));
				ui.changed = false;
			}
		}
		clientSendThread.join();
		clientReceiveThread.join();
		break;
	}
	default:
		std::cout << "Invalid Connection Type";
		return;
	}

	ui.cleanup();
}
