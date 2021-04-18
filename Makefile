NAME	= webserv

SRCDIR	= ./srcs/
INCDIR	= ./includes/

# SRC		=	Client.cpp \
# 			Config.cpp \
# 			ConfigGroup.cpp \
# 			Http.cpp \
# 			Method.cpp \
# 			Server.cpp \
# 			Socket.cpp \
# 			utils.cpp \
# 			Webserver.cpp \
# 			main.cpp

# SRCS	= $(addprefix $(SRCDIR), $(SRC))
SRCNAMES = $(shell ls $(SRCDIR) | grep -E ".+\.cpp")
OBJS	= $(SRCS:.cpp=.o)

BUILDDIR = ./build/
BUILDOBJS = $(addprefix $(BUILDDIR), $(SRCNAMES:.cpp=.o))

CC		= clang++
CCFLAG	= -std=c++98 -g -fsanitize=address -Wall -Wextra -Werror
RM		= rm -f

%.o:		%.cpp
			$(CC) $(CCFLAG) -I$(INCDIR) -c $< -o $@

all:		$(BUILDDIR) $(NAME)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)%.o:$(SRCDIR)%.cpp
	$(CC) $(CCFLAGS) -I$(INCDIR) -o $@ -c $<

$(NAME):	$(BUILDOBJS)
			$(CC) $(CCFLAG) -I$(INCDIR) -o $(NAME) $(BUILDOBJS)


clean:
			$(RM) $(BUILDOBJS)
			rm -rf $(BUILDDIR)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
