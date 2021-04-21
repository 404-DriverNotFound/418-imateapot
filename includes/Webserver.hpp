#pragma once

#include "ConfigGroup.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Client.hpp"

#include <vector>
#include <string>
#include <unistd.h>

#define READ_BUFFER 1024

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

		void acceptRequest(Socket &sock);
		void readRequest(Client &client);
		void handleResponse(Client &client);

		void selectErrorHandling(std::vector<int>& err_index);

	public:
		Webserver(const std::string &path, uint32_t max_connection);
		void startServer();

		class SelectException: public std::exception
		{
			virtual const char *what() const throw(); 
		};

		class SocketReadException: public std::exception
		{
			virtual const char *what() const throw(); 
		};
};
