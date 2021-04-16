#include "Webserver.hpp"

Webserver::Webserver(const std::string &path, uint32_t max_connection) : _configs(ConfigGroup(path, max_connection))
{
	std::vector<u_int16_t> server_ports;

	// server 만들어주기
	for (int i = 0; i < _configs.getServerCnt(); i++)
	{
		_servers.push_back(Server(_configs.getConfig(i)));
		uint16_t port = _servers[i].getPort();
		if (std::find(server_ports.begin(), server_ports.end(), port) == server_ports.end())
			server_ports.push_back(port);
	}

	for (int i = 0; i < server_ports.size(); i++)
	{
		
	}
}

