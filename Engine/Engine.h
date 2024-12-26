#pragma once
#include <string>
#include <UI.h>
#include <NetworkConnection.h>
#include <unordered_map>

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

	int generateRandomId();

	// Parameterized constructor
	Engine(ConnectionType connectionType, const char* host, int port);

	void initialize(Engine::ConnectionType connectionType, const char* host, int port);

	void run();

private:
	void update();
	void updateGameObjects(SDL_Event event);
	std::vector<unsigned char> serializeGameObject(Sprite gameObject);
	Sprite deserializeGameObject(std::vector<unsigned char> serializedGameObject);
	std::vector<unsigned char> serializeGameObjects(const std::unordered_map<unsigned int, Sprite>& gameObjects);
	std::unordered_map<unsigned int, Sprite> deserializeGameObjects(const std::vector<unsigned char>& serializedGameObjects);
	UI ui;
	NetworkConnection* connection;
	ConnectionType connectionType;
	const char* host;
	int port;
	float spawnX = 100;
	bool running = true;
	unsigned int playerId = 0;
	std::unordered_map<unsigned int, Sprite> players;
	std::unordered_map<unsigned int, Sprite> gameObjects;

};

