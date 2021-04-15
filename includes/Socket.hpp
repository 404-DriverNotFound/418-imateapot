#pragma once
#ifdef __linux__
	#include <stdint.h>
#endif
#include <sys/socket.h>

class Socket : public sockaddr
{
	int fd;

public:
	void bind();
	void accept();
	void listen();
};
