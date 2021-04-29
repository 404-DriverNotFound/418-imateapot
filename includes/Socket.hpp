#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>
#include <exception>
#include <cstring>
#include <unistd.h>

class Socket
{
	private:
		sockaddr_in _sockaddr;
		uint16_t	_port;
		int 		_fd;

	public:
		Socket(uint16_t port, uint32_t max_connection);

		int getFd();
		int getPort();

		class SocketCreationException: public std::exception
		{
			virtual const char *what() const throw(); 
		};

		class BindException: public std::exception
		{
			virtual const char *what() const throw(); 
		};
		
		class ListenException: public std::exception
		{
			virtual const char *what() const throw(); 
		};
};
