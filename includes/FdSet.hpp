#pragma once
#include "webserv.hpp"

struct FdSet : public fd_set
{
public:
	FdSet();
	~FdSet();
	bool get(uint32_t fd) const;
	void set(uint32_t fd);
	void del(uint32_t fd);
};
