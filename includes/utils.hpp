#pragma once
#ifdef __linux__
	#include <stdint.h>
#endif
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> ft_split(std::string str, char delim);

#ifndef FT_FD_SETSIZE
#define FT_FD_SETSIZE 1024
#endif

#define FT_NBBY 8
typedef uint32_t ft_fd_mask;
#define FT_NFDBITS ((unsigned)(sizeof(ft_fd_mask) * FT_NBBY))
#define HOWMANY(x, y) (((x) + (y) - 1) / (y))

/* FD_SET */
static __inline void
__fd_set(int fd, fd_set *p)
{
    p->fds_bits[fd / FT_NFDBITS] |= (1U << (fd % FT_NFDBITS));
}

#define FT_FD_SET(n, p) __fd_set((n), (p))

/* FD_CLR */
static __inline void
__fd_clr(int fd, fd_set *p)
{
	p->fds_bits[fd / FT_NFDBITS] &= ~(1U << (fd % FT_NFDBITS));
}
#define FT_FD_CLR(n, p)	__fd_clr((n), (p))

/* FD_ISSET */
static __inline int
__fd_isset(int fd, const fd_set *p)
{
	return (p->fds_bits[fd / FT_NFDBITS] & (1U << (fd % FT_NFDBITS)));
}
#define FT_FD_ISSET(n, p)	__fd_isset((n), (p))

/* FD_ZERO */
#define	FT_FD_ZERO(p) do {					\
	fd_set *_p = (p);				\
	size_t _n = HOWMANY(FD_SETSIZE, FT_NFDBITS);	\
							\
	while (_n > 0)					\
		_p->fds_bits[--_n] = 0;			\
} while (0)

#define        FT_S_ISTYPE(mode, mask)        (((mode) & S_IFMT) == (mask))
#define        FT_S_ISDIR(mode)         FT_S_ISTYPE((mode), S_IFDIR)