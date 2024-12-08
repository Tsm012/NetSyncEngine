#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <Engine.h>

#include "Main.h"

int main(int argc, char* argv[])
{
	std::map<std::string, std::string> args =
		parseCommandLineArguments(argc, argv);

	const char* host = getHost(args);
	int port = getPort(args);

	Engine(Engine::Client, host, port).run();

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
