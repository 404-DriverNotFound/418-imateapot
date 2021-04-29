#pragma once

#include "Client.hpp"
#include "Config.hpp"

class CGI
{
    private:
        Config      &_config;
        std::string &_file_path;


    public:
        CGI(Config &conf, std::string &file_path);
};