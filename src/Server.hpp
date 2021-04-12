#pragma once
#include <string>
#include <vector>
#include <list>
#include "Client.hpp"
#include "Config.hpp"
#include "ConfigServer.hpp"

class Server
{
	ConfigServer&			config_server;			// 웹서버 설정이 하위 설정을 벡터<벡터<설정>>으로 갖는다면 폐기
	std::vector<Config>		config_locations;		
	std::list<Client>		clients;
};