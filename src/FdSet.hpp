#include <sys/select.h>
#include <iostream>

struct FdSet : public fd_set
{
public:
	FdSet();
	~FdSet();
	bool get(uint32_t fd) const;
	void set(uint32_t fd);
	void del(uint32_t fd);
};
