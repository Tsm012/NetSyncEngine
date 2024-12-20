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

	gameObjects.push_back(Sprite(ui.loadTexture("Player.bmp"), SDL_FRect{ 100, 100, 175, 100 }, 15));
}
void Engine::run()
{
	std::thread sendThread(&NetworkConnection::start, connection);

	while (!connection->connected && !connection->hasError)
	{
		Sleep(100);
	}

	std::thread receiveThread(&NetworkConnection::receiveData, connection);

	while (running && connection->connected && !connection->hasError)
	{
		update();
	}

	sendThread.join();
	receiveThread.join();
	ui.cleanup(gameObjects);
}

void Engine::update()
{
	auto message = connection->getChannel().fetchReceivedData();

	//Receive updates
	if (message.has_value())
	{
		int count = 0;
		for (Sprite& object : deserializeGameObjects(message.value()))
		{
			gameObjects[count].boundingBox = object.boundingBox;
			count++;
		}
	}
	SDL_Event event = ui.getInput();

	if (event.type == SDL_EVENT_QUIT)
	{
		running = false;
	}
	else if (event.type == SDL_EVENT_KEY_DOWN)
	{
		updateGameObjects(event);
		connection->getChannel().setDataToSend(serializeGameObjects(gameObjects));
	}

	ui.render(gameObjects);
}

void Engine::updateGameObjects(SDL_Event event)
{
	switch (event.key.scancode)
	{
	case SDL_SCANCODE_UP:
		gameObjects[0].boundingBox.y -= gameObjects[0].moveStep;
		break;
	case SDL_SCANCODE_DOWN:
		gameObjects[0].boundingBox.y += gameObjects[0].moveStep;
		break;
	case SDL_SCANCODE_LEFT:
		gameObjects[0].boundingBox.x -= gameObjects[0].moveStep;
		break;
	case SDL_SCANCODE_RIGHT:
		gameObjects[0].boundingBox.x += gameObjects[0].moveStep;
		break;
	}
}

std::vector<unsigned char> Engine::serializeGameObjects(const std::vector<Sprite>& gameObjects)
{
	std::vector<unsigned char> serializedData;
	for (const Sprite& object : gameObjects)
	{
		size_t dataSize = sizeof(object.boundingBox) + sizeof(object.moveStep);
		std::vector<unsigned char> objData(dataSize);
		size_t offset = 0;
		std::memcpy(objData.data() + offset, &object.boundingBox, sizeof(object.boundingBox));
		offset += sizeof(object.boundingBox);
		std::memcpy(objData.data() + offset, &object.moveStep, sizeof(object.moveStep));
		offset += sizeof(object.moveStep);

		serializedData.insert(serializedData.end(), objData.begin(), objData.end());
	}
	return serializedData;
}

std::vector<Sprite> Engine::deserializeGameObjects(const std::vector<unsigned char>& serializedGameObjects)
{
	std::vector<Sprite> deserializedObjects;
	size_t offset = 0;

	while (offset < serializedGameObjects.size())
	{
		// Deserialize one object
		size_t objectSize = sizeof(SDL_FRect) + sizeof(int);
		if (offset + objectSize > serializedGameObjects.size()) break;

		SDL_FRect boundingBox;
		std::memcpy(&boundingBox, &serializedGameObjects[offset], sizeof(SDL_FRect));
		offset += sizeof(SDL_FRect);

		int moveStep;
		std::memcpy(&moveStep, &serializedGameObjects[offset], sizeof(int));
		offset += sizeof(int);

		// Add the deserialized object to the list
		deserializedObjects.emplace_back(nullptr, boundingBox, moveStep);
	}

	return deserializedObjects;
}

