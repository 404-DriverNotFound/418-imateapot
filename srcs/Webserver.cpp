#include "Webserver.hpp"

Webserver::Webserver(const std::string &path, uint32_t max_connection) : _configs(ConfigGroup(path, max_connection))
{
	// server 만들어주기
	for (int i = 0; i < _configs.getServerCnt(); i++)
		_servers.push_back(Server(_configs.getConfig(i)));
}

