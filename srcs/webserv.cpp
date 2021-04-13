#include <iostream>
#include <fstream>
#include <vector>
#include "webserv.hpp"

void addVector(std::vector< std::vector<int> > &test) {
	std::vector<int> a;
	a.push_back(1);
	test.push_back(a);
}

int main(int argc, char **argv)
{
	std::string a(argv[1]);
	ConfigGroup obj(a, 20);
	std::vector<Config> test1 = obj.getConfig(0);
	std::vector<Config> test2 = obj.getConfig(1);

	std::cout << test1[0].location_path << std::endl;
	std::cout << test2[1].location_path << std::endl;
	return 0;
}
