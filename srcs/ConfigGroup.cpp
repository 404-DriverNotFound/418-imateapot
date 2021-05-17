#include "webserv.hpp"

/**
 * parseServer
 * conf file을 gnl을 통해 읽은 뒤, server 단에 맞추어 _configs에 저장
 * @param  {std::ifstream} config_file : argv[1](config file 경로)
 * @param  {std::string} line          : gnl을 통해 읽은 라인
 */
void ConfigGroup::parseServer(int config_fd, std::string &line, int &gnl_status)
{
	bool is_location_start = false;
	bool is_root_set = false;
	std::vector<Config> server_vector;
	Config server_config;

	gnl_status = ft_getline(config_fd, line);

	while (gnl_status == GNL_OK)
	{
		if (isBlankLine(line))
		{
			gnl_status = ft_getline(config_fd, line);
			continue;
		}
		if (line[0] != '\t')
			break;
		if (line[1] == '\t')
			throw ConfigGroup::ConfigFormatException();

		std::vector<std::string> split = ft_split(line.substr(1), ' ');

        if (!split[0].compare("location"))
		{
            if (split.size() > 2)
                throw ConfigGroup::ConfigFormatException();
			is_location_start = true;
            server_vector.push_back(parseLocation(config_fd, line, split[1], server_config, gnl_status));
			continue;
		}
		if (is_location_start ||
			(split[0].compare("method") && split.size() > 2))
                throw ConfigGroup::ConfigFormatException();

		if (!is_root_set && split[0].compare("root"))
			throw ConfigGroup::ConfigFormatException();

        server_config.parseConfig(split, false);
		is_root_set = true;
		gnl_status = ft_getline(config_fd, line);
    }
	if (server_config.cgi_path.empty() != server_config.cgi_extension.empty())
		throw ConfigGroup::ConfigFormatException();

	if (!checkDupLocation(server_vector))
		throw ConfigGroup::ConfigFormatException();
    server_vector.push_back(server_config);

	_configs.push_back(server_vector);
}

/**
 * parseLocation
 * server단 안에 있는 location부분을 파싱하는 함수
 * @param  {std::ifstream} config_file : config file 경로
 * @param  {std::string} line          : gnl을 통해 읽은 라인
 * @param  {std::string} loc           : line을 split을 통해 나눠진 location_path
 * @param  {Config} server_config      : Config 구조체
 * @return {Config}                    : location의 정보가 들어간 Config 구조체
 */
Config ConfigGroup::parseLocation(int config_fd, std::string &line, std::string &loc, Config &server_config, int &gnl_status)
{
	bool is_root_set = false;
    Config location_config(server_config, loc);

	gnl_status = ft_getline(config_fd, line);

	while (gnl_status == GNL_OK)
    {
        if (isBlankLine(line))
		{
			gnl_status = ft_getline(config_fd, line);
            continue;
		}
		
        if (line[0] != '\t' || line[1] != '\t')
			break;
		if (line[2] == '\t')
			throw ConfigGroup::ConfigFormatException();

		std::vector<std::string> split = ft_split(line.substr(2), ' ');

		if (split[0].compare("method") && split.size() > 2)
			throw ConfigGroup::ConfigFormatException();

		if (!is_root_set && split[0].compare("root"))
			throw ConfigGroup::ConfigFormatException();

        location_config.parseConfig(split, true);
		is_root_set = true;
		gnl_status = ft_getline(config_fd, line);
    }
	if (location_config.cgi_path.empty() != location_config.cgi_extension.empty())
		throw ConfigGroup::ConfigFormatException();
    return (location_config);
}

/**
 * ConfigGroup
 * config file 전체를 파싱하는 생성자
 * @param  {std::string} path        : config file 경로
 * @param  {uint32_t} max_connection : 서버에서 받을 수 있는 최대 연결 수 (기본값 20)
 */
ConfigGroup::ConfigGroup(const std::string &path, uint32_t max_connection = 20): _max_connection(max_connection)
{
	int gnl_status;
	int config_fd = open(path.c_str(), O_RDONLY);

    if (config_fd < 0)
		throw ConfigGroup::NoConfigFileException();

	std::string line;
	gnl_status = ft_getline(config_fd, line);

    while (gnl_status == GNL_OK)
    {
        if (isBlankLine(line))
		{
			gnl_status = ft_getline(config_fd, line);
            continue;
		}
        if (line.compare("server"))
            throw ConfigGroup::ConfigFormatException();
        this->parseServer(config_fd, line, gnl_status);
    }
	close(config_fd);
	if (!checkDupServer())
		throw ConfigGroup::ConfigFormatException();
}

ConfigGroup::~ConfigGroup() {}

/* --------- getter --------- */

int ConfigGroup::getServerCnt()
{
    return (this->_configs.size());
}

uint32_t ConfigGroup::getMaxConnection()
{
    return (this->_max_connection);
}

std::vector<Config> &ConfigGroup::getConfig(int index)
{
    return (this->_configs[index]);
}

/* -------------------------- */

/**
 * checkDupLocation
 *
 * @param  {std::vector<Config>} server_vector : 1개의 server 단의 정보를 가진 vector
 * @return {bool}                              : location_path가 중복일 시, false
 */
bool ConfigGroup::checkDupLocation(std::vector<Config> server_vector)
{
	int size = server_vector.size();

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (server_vector[i].location_path == server_vector[j].location_path)
				return false;
		}
	}
	return true;
}

/**
 * checkDupServer
 *
 * @return {bool}  : 여러 개의 server 단의 port와 server_name을 비교하여 중복 검사 실시
 *                   중복이 있을 시, false
 */
bool ConfigGroup::checkDupServer()
{
	int size = getServerCnt();

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if ((_configs[i][0].port == _configs[j][0].port &&
				_configs[i][0].server_name == _configs[j][0].server_name))
					return false;
		}
	}
	return true;
}

const char *ConfigGroup::NoConfigFileException::what() const throw()
{
    return ("NoConfigFileException: there is no file!");
}

const char *ConfigGroup::ConfigFormatException::what() const throw()
{
    return ("ConfigFormatException: Invalid format!");
}
