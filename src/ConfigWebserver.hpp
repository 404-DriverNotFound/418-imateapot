#pragma once
#include <vector>
#include "Path.hpp"
#include "Config.hpp"
#include "ConfigServer.hpp"

class ConfigWebserver
{
	uint32_t							max_connection;
	std::vector< std::vector<Config> >	configs;

	public:
		ConfigWebserver(const Path &);
};
