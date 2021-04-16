#include "Webserver.hpp"
/**
 * Webserver 
 * @brief  전체 웹서버 인스턴스를 만드는 생성자
 * @param  {const std::string} path  : 설정 파일의 경로
 * @param  {uint32_t} max_connection : 최대 접속 가능수
 */
Webserver::Webserver(const std::string &path, uint32_t max_connection) : _configs(ConfigGroup(path, max_connection))
{
	std::vector<u_int16_t> server_ports;

	for (int i = 0; i < this->_configs.getServerCnt(); i++)
	{
		this->_servers.push_back(Server(this->_configs.getConfig(i)));
		uint16_t port = this->_servers[i].getPort();
		if (std::find(server_ports.begin(), server_ports.end(), port) == server_ports.end())
			server_ports.push_back(port);
	}
	
	FT_FD_ZERO(&(this->_fd_read));
	for (int i = 0; i < server_ports.size(); i++)
	{
		this->_socks.push_back(Socket(server_ports[i]));
		FT_FD_SET(this->_socks[i].getFd(),&(this->_fd_read));
	}
}

/**
 * Webserver::start_server 
 * @brief  Webserver 내 Server를 바탕으로 서버를 시작하는 함수. 이 함수에서 프로젝트 유일하게 select가 작동됨.
 */
void Webserver::start_server()
{
	fd_set temp_fd_read;
	int state;

	while (1)
	{
		temp_fd_read = this->_fd_read;
		state = select(FT_FD_SETSIZE + 1, &temp_fd_read, NULL, NULL, NULL);
		
		switch (state)
		{
		case -1:
			throw Webserver::SelectException();
		case 0:
			continue;
		default:
			for (int i = 0; i < this->_socks.size(); i++)
				if (FT_FD_ISSET(this->_socks[i].getFd(),&(temp_fd_read)))
					accept_request(this->_socks[i]);

		}

	}
}

/**
 * Webserver::accept_request
 * @brief  select를 통해 들어온 특정 소켓의 데이터를 처리하는 함수
 * @param  {Socket} sock : select를 통해 데이터가 들어온 것을 확인한 소켓
 */
void Webserver::accept_request(Socket &sock)
{
	
}

const char *Webserver::SelectException::what() const throw()
{
	return "SelectException: fail to run select!";
}