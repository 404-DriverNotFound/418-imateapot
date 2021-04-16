#include "webserv.hpp"

/**
 * Socket::Socket
 * @brief  Socket 생성자
 * @param  {uint16_t} port : 만들 소켓에 대한 포트번호
 */
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

int Socket::getFd()
{
	return this->_fd;
}

const char *Socket::SocketCreationException::what() const throw()
{
	return "SocketCreationException: Failed to get socket fd!";
}

const char *Socket::BindException::what() const throw()
{
	return "BindException: Failed to bind!";
}