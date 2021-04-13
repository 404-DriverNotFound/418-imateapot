#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include "Path.hpp"
#include "Config.hpp"

class ConfigGroup
{
	private:
		uint32_t							_max_connection; // def = 20
		std::vector< std::vector<Config> >	_configs;

	public:
		ConfigGroup(const std::string &path, uint32_t max_connection = 20); // constructor
		~ConfigGroup();

		/* --------- getter ---------*/
		int getSeverCnt(); // config length return
		uint32_t getMaxConnection();
		std::vector<Config> &getConfig(int index);

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