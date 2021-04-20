#include "webserv.hpp"

/**
 * Server::Server 
 * @brief  Server 생성자
 * @param  {std::vector<Config>} config : 생성할 Server에 대한 config vector
 */
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