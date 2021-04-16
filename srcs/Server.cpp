#include "Server.hpp"

Server::Server(std::vector<Config> &config):
	_config(config),
	_server_name(config[0].server_name),
	_port(config[0].port)
{

}

uint16_t Server::getPort()
{
	return this->_port;
}