#include "Socket.hpp"

/**
 * Socket::Socket
 * @brief  Socket 생성자
 * @param  {uint16_t} port : 만들 소켓에 대한 포트번호
 */
Socket::Socket(uint16_t port, uint32_t max_connection): _port(port)
{
	if ((this->_sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw Socket::SocketCreationException();
	memset(&(this->_sockaddr), 0x00, sizeof(sockaddr_in));
	this->_sockaddr.sin_family = AF_INET;
	this->_sockaddr.sin_addr.s_addr = ft_htonl(INADDR_ANY);
	this->_sockaddr.sin_port = ft_htons(this->_port);
	_ip = ft_inet_ntoa(this->_sockaddr.sin_addr.s_addr);

	if (bind(this->_sock_fd, reinterpret_cast<const sockaddr *>(&(this->_sockaddr)), sizeof(sockaddr)) < 0)
		throw Socket::BindException();
	if (listen(this->_sock_fd, max_connection) == -1)
		throw Socket::ListenException();
	fcntl(this->_sock_fd, F_SETFL, O_NONBLOCK);
}

int Socket::getSockFd()
{
	return this->_sock_fd;
}

int Socket::getPort()
{
	return this->_port;
}

std::string Socket::getIp()
{
	return this->_ip;
}

const char *Socket::SocketCreationException::what() const throw()
{
	return "SocketCreationException: Failed to get socket fd!";
}

const char *Socket::BindException::what() const throw()
{
	return "BindException: Failed to bind!";
}

const char *Socket::ListenException::what() const throw()
{
	return "ListenException: Failed to listen!";
}
