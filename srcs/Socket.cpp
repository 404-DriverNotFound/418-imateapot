#include "webserv.hpp"

Socket::Socket(uint16_t port): _port(port)
{
	if ((this->_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw Socket::SocketCreationException();
	this->sin_family = AF_INET;
	this->sin_addr.s_addr = htonl(INADDR_ANY);
	this->sin_port = htons(this->_port);

	if (bind(this->_fd, reinterpret_cast<const sockaddr *>(this), sizeof(sockaddr)) < 0)
		throw Socket::BindException();
}

const char *Socket::SocketCreationException::what() const throw()
{
	return "SocketCreationException: Failed to get socket fd!";
}

const char *Socket::BindException::what() const throw()
{
	return "BindException: Failed to bind!";
}