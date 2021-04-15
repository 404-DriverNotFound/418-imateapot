#include <iostream>
#include <fstream>
#include <vector>
#include "webserv.hpp"

int main(int argc, char **argv)
{
	/*
	std::string a(argv[1]);
	ConfigGroup obj(a, 20);
	std::vector<Config> test1 = obj.getConfig(0);
	std::vector<Config> test2 = obj.getConfig(1);

	std::cout << test1[0].location_path << std::endl;
	std::cout << test2[1].location_path << std::endl;
	*/

	Client a;

	std::vector<std::string> strs;
	strs.push_back("GET /abc/def?a=1&b=2 HTTP/1.1");
	strs.push_back("GET http://localhost/abc/def?a=1&b=2 HTTP/1.1");
	strs.push_back("POST / HTTP/1.1");
	strs.push_back("DELETE / HTTTP/1.1");
	for (int i = 0; i < strs.size(); i++)
	{
		try
		{
			a.recvStartLine(strs[i]);
			StartLineReq &b = a._request.getStartLine();
			std::cout << b.method << std::endl;
			std::cout << b.path << std::endl;
			std::cout << b.query_string << std::endl;
			std::cout << b.protocol << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}	
	return 0;
}
