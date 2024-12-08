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

	void update();

private:
	UI ui;
	NetworkConnection* connection;
	ConnectionType connectionType;
	const char* host;
	int port;
};
