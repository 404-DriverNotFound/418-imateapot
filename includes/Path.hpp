#pragma once
#ifdef __linux__
	#include <stdint.h>
#endif
#include <vector>
#include <string>

class Path
{
private:
	std::vector<std::string> segments;

public:
	const char *getPath() const;
	void setPath(const std::string &);
	Path &operator+(Path &);
	Path &operator--();
	Path &operator=(const std::string &);
};
