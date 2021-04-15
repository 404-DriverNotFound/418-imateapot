#pragma once
#ifdef __linux__
	#include <stdint.h>
#endif
#include <string>
#include <vector>
#include <list>
#include "Client.hpp"
#include "Config.hpp"

class Server
{
	private:
		std::vector<Config> &_config;
		std::list<Client> _clients;

	public:
		/*------------ getter ------------*/

		/*------------ constructor ------------*/
		Server(std::vector<Config> &config);
};
