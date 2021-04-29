#include "webserv.hpp"

int main(int argc, char **argv)
{
	// uint32_t max_connection = 20;
	// if (argc == 3)
	// 	max_connection = ft_atoi(argv[2]);
	// if (argc < 2 || argc > 3)
	// 	return 1;
	// try
	// {
	// 	Webserver server(argv[1], max_connection);
	// 	server.startServer();
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// 	return 1;
	// }

	std::cout << ft_itos(1000) << std::endl;
	std::cout << ft_itos(1) << std::endl;
	std::cout << ft_itos(223948) << std::endl;
	std::cout << ft_itos(0) << std::endl;
	std::cout << ft_itos(10009) << std::endl;
	std::cout << ft_itos(4584757) << std::endl;
	std::cout << ft_itos(10) << std::endl;
	std::cout << ft_itos(-1347673) << std::endl;
	std::cout << ft_itos(283274589) << std::endl;
	return 0;
}