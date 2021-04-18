#pragma once

#include "utils.hpp"

#include "Client.hpp"
#include "Config.hpp"
#include "ConfigGroup.hpp"
#include "Http.hpp"
#include "Method.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Webserver.hpp"

#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>

#ifdef __linux__
	#include <stdint.h>
#endif

#include <sys/types.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>
