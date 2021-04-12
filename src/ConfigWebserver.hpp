#pragma once
#include <vector>
#include "Path.hpp"
#include "ConfigServer.hpp"

/*
*Webserver
*>> Server
*>>>> Location
*>>>> Location
*>>>> Location
*>> Server
*>>>> Location
*>>>> Location
*>> Server
*>>>> Location
*>>>> Location
*>>>> Location
*	.
*	.
*	.
*/
class ConfigWebserver
{
	uint32_t					max_connection;
	std::vector<ConfigServer>	config_servers;		
	// ConfigServer 클래스를 폐기하고, 벡터<벡터<Config>>로 할 수도 있음
	
	public:
								ConfigWebserver(const Path&);
};