#include <iostream>
#include <fstream>
#include <vector>
#include "webserv.hpp"

int main(int argc, char **argv)
{
	std::string a(argv[1]);
	ConfigGroup obj(a, 20);
	std::vector<Config> test1 = obj.getConfig(0);

	std::cout << test1[0].location_path << std::endl;
	return 0;
}
