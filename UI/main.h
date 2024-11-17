#pragma once

enum Mode { 
	SERVER_MODE = 1, 
	CLIENT_MODE = 2 
};
Mode SelectNetworkMode();
