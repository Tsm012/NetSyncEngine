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
	std::vector<unsigned char> serializeVector(const std::vector<Object>& vec);
	std::vector<Object> deserializeVector(const std::vector<unsigned char>& serializedData);
	UI ui;
	NetworkConnection* connection;
	ConnectionType connectionType;
	const char* host;
	int port;
	bool running = true;
	std::vector<Object> gameObjects;
};

