#include "webserv.hpp"

int main(int argc, char **argv)
{
	uint32_t max_connection = 20;
	if (argc == 3)
		max_connection = atoi(argv[2]);
	if (argc < 2 || argc > 3)
		return 1;
	Webserver server(argv[1], max_connection);
	server.startServer();
}