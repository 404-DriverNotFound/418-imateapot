#pragma once
#include <sys/socket.h>

class Socket : public sockaddr
{
	int fd;

public:
	void bind();
	void accept();
	void listen();
};
