#include "UI.h"
#include <Client.h>
#include <Server.h>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <thread>

std::map<std::string, std::string> parseCommandLineArguments(int argc,
                                                             char *argv[]) {
  std::map<std::string, std::string> args;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    size_t pos = arg.find('=');
    if (pos != std::string::npos) {
      std::string key = arg.substr(0, pos);
      std::string value = arg.substr(pos + 1);
      args[key] = value;
    }
  }
  return args;
}

int main(int argc, char *argv[]) {

  std::map<std::string, std::string> args =
      parseCommandLineArguments(argc, argv);

  if (args.find("-host") != args.end()) {
    std::cout << "Host: " << args["-host"] << std::endl;
  } else {
    std::cout << "Host argument not provided!" << std::endl;
  }

  int port = 0;
  
  if (args.find("-port") != args.end()) {
    std::cout << "Port: " << args["-port"] << std::endl;
    port = std::stoi(args["-port"]);
  } else {
    std::cout << "Port argument not provided!" << std::endl;
  }

  UI ui;

  if (!ui.initialize("Client")) {
    return 1;
  }

  Client client;
  if (client.connect("localhost", port)) {
    ui.run(client);
  }

  ui.cleanup();

  return 0;
}
