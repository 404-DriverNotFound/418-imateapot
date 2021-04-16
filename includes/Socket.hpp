#pragma once
#include "webserv.hpp"

class Socket : public sockaddr_in
{
	private:
		uint16_t	_port;
		int 		_fd;

	public:
		Socket(uint16_t port);

		int getFd();
		// void bind();
		void accept();
		void listen();

		class SocketCreationException: public std::exception
		{
			virtual const char *what() const throw(); 
		};

		class BindException: public std::exception
		{
			virtual const char *what() const throw(); 
		};
};
