#pragma once
#include <string>

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

	void run();

private:
	ConnectionType connectionType;
	const char* host;
	int port;
};
