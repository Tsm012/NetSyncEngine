#include <Client.h>
#include <Server.h>

#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <thread>

#include <UI.h>
#include "Main.h"

int main(int argc, char* argv[])
{
	std::map<std::string, std::string> args =
		parseCommandLineArguments(argc, argv);

	const char* host = getHost(args);
	int port = getPort(args);

	UI ui;

	if (!ui.initialize("Client"))
	{
		return 1;
	}

	Client client;
	std::thread clientSendThread(&Client::connect, &client, host, port);
	while (!client.getChannel().connected)
	{
		Sleep(100);
	}

	std::thread clientReceiveThread(&Client::receiveData, &client);

	while (ui.running)
	{
		auto rect = ui.update();
		if (ui.changed)
		{
			unsigned char byteArray[sizeof(SDL_FRect)];
			std::memcpy(byteArray, &rect, sizeof(SDL_FRect));
			client.getChannel().setDataToSend(byteArray, sizeof(SDL_FRect));
			ui.changed = false;
		}
	}
	clientSendThread.join();
	clientReceiveThread.join();

	ui.cleanup();

	return 0;
}

std::map<std::string, std::string> parseCommandLineArguments(int argc,
	char* argv[])
{
	std::map<std::string, std::string> args;
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		size_t pos = arg.find('=');
		if (pos != std::string::npos)
		{
			std::string key = arg.substr(0, pos);
			std::string value = arg.substr(pos + 1);
			args[key] = value;
		}
	}
	return args;
}

int getPort(std::map<std::string, std::string>& args)
{
	if (args.find("-port") != args.end())
	{
		std::cout << "Port: " << args["-port"] << std::endl;
		return std::stoi(args["-port"]);
	}
	std::cout << "Port argument not provided!" << std::endl;
	exit(0);
}

const char* getHost(std::map<std::string, std::string>& args)
{
	if (args.find("-host") != args.end())
	{
		std::cout << "Host: " << args["-host"] << std::endl;
		return args["-host"].c_str();
	}
	std::cout << "Host argument not provided!" << std::endl;
	exit(0);
}
