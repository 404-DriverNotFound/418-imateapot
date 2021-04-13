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
	std::vector<Config> vec = obj.getConfig(0);

	std::cout << vec[0].root << std::endl;
	return 0;
}
