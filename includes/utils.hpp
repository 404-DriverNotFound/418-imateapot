#pragma once

#ifdef __linux__
	#include <stdint.h>
#endif
#include <string>
#include <vector>
#include <netinet/in.h>
#include <netinet/ip.h>

#define GNL_BUFFER 1024
#define GNL_OK 1
#define GNL_EOF 0

class GNLException: public std::exception
{
	virtual const char *what() const throw();
};

int							ft_getline(int fd, std::string &line);
std::vector<std::string>	ft_split(std::string str, char delim);

bool			isBlankLine(const std::string &line);
void			ft_trim(std::string &str, const std::string cut);
int				ft_atoi(const std::string &str);
std::string		ft_itos(int num);
std::string		ft_ultohex(unsigned long num);
unsigned long	ft_uhextol(const std::string &str);
std::string		getCurrentTime();
std::string 	getHTTPTimeFormat(time_t time);
bool 			isFilePath(const std::string &path);
bool			isDirPath(const std::string &path);
std::string		getStatusStr(uint16_t code);
std::string		ft_inet_ntoa(unsigned int addr);
uint16_t		ft_htons(uint16_t port);
u_int32_t		ft_htonl(unsigned long int addr);

#ifndef FT_FD_SETSIZE
#define FT_FD_SETSIZE 256
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
