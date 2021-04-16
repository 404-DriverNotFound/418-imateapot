#pragma once

#include "ConfigGroup.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Client.hpp"

#include <vector>
#include <string>

class Webserver
{
	private:
		ConfigGroup			_configs;
		std::vector<Server>	_servers;
		fd_set				_fd_exception;
		fd_set				_fd_read;
		fd_set				_fd_write;
		std::vector<Socket>	_socks;
		std::vector<Client>	_clients;

	public:
		Webserver(const std::string &path, uint32_t max_connection);
		void start_server();
		void accept_request(Socket &sock);

		class SelectException: public std::exception
		{
			virtual const char *what() const throw(); 
		};
};
