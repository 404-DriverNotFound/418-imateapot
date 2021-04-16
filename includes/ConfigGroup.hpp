#pragma once
#ifdef __linux__
	#include <stdint.h>
#endif
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include "Config.hpp"
#include "utils.hpp"

class ConfigGroup
{
	private:
		uint32_t							_max_connection; // def = 20
		std::vector< std::vector<Config> >	_configs;

		void parseServer(std::ifstream &config_file, std::string &line);
		Config parseLocation(std::ifstream &config_file, std::string &line, std::string &loc, Config &server_config);

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

bool isBlankLine(const std::string &line);
