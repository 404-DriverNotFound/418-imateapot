#pragma once
#ifdef __linux__
	#include <stdint.h>
#endif

enum e_method
{
	GET,
	HEAD,
	PUT,
	POST,
	TRACE,
	DELETE,
};
