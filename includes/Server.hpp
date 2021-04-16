#pragma once
#include "webserv.hpp"

class Server
{
	private:
		std::vector<Config> &_config;
		std::string			_server_name;
		uint16_t			_port;

	public:
		/*------------ getter ------------*/
		uint16_t getPort();

		/*------------ constructor ------------*/
		Server(std::vector<Config> &config);
};
