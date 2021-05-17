#pragma once

#include "Config.hpp"
#include "utils.hpp"

#include <fstream>
#include <vector>
#include <string>

class ConfigGroup
{
	private:
		uint32_t							_max_connection; // def = 20
		std::vector< std::vector<Config> >	_configs;

		void parseServer(int config_fd, std::string &line, int &gnl_status);
		Config parseLocation(int config_fd, std::string &line, std::string &loc, Config &server_config, int &gnl_status);

	public:
		ConfigGroup(const std::string &path, uint32_t max_connection); // constructor
		~ConfigGroup();

		/* --------- getter ---------*/
		int getServerCnt();
		uint32_t getMaxConnection();
		std::vector<Config> &getConfig(int index);

		bool checkDupLocation(std::vector<Config> server_vector);
		bool checkDupServer();

		class NoConfigFileException: public std::exception
		{
			virtual const char *what() const throw();
		};
		class ConfigFormatException: public std::exception
		{
			virtual const char *what() const throw();
		};
};
