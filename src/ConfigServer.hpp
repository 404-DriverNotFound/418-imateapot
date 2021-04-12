#pragma once
#include "Config.hpp"
#include "Path.hpp"
#include <iostream>
#include <vector>
/*
Server
>> Location
>> Location
>> Location
*/
class ConfigServer: public Config
{
	std::vector<Config>	config_locations;
};