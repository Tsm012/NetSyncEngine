#pragma once
#include <string>
#include <UI.h>
#include <NetworkConnection.h>

class Engine
{
public:
	enum ConnectionType
	{
		Server,
		Client
	};

	// Default constructor
	Engine();

	// Parameterized constructor
	Engine(ConnectionType connectionType, const char* host, int port);

	void initialize(Engine::ConnectionType connectionType, const char* host, int port);

	void run();

private:
	void update();
	void updateGameObjects(SDL_Event event);
	std::vector<unsigned char> serializeGameObjects(const std::vector<Sprite>& gameObjects);
	std::vector<Sprite> deserializeGameObjects(const std::vector<unsigned char>& serializedGameObjects);
	UI ui;
	NetworkConnection* connection;
	ConnectionType connectionType;
	const char* host;
	int port;
	bool running = true;
	std::vector<Sprite> gameObjects;
};

