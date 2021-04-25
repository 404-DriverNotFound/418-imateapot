NAME		= webserv

SRCDIR		= ./srcs/
INCDIR		= ./includes/

SRCNAMES	= $(shell ls $(SRCDIR) | grep -E ".+\.cpp")
OBJS		= $(SRCS:.cpp=.o)

BUILDDIR	= ./build/
BUILDOBJS	= $(addprefix $(BUILDDIR), $(SRCNAMES:.cpp=.o))

CC			= clang++
CCFLAG		= -std=c++98 -g -fsanitize=address -Wall -Wextra -Werror
RM			= rm -f

all:			$(BUILDDIR) $(NAME)

$(BUILDDIR):
				mkdir -p $(BUILDDIR)

$(BUILDDIR)%.o:	$(SRCDIR)%.cpp
				$(CC) $(CCFLAG) -I$(INCDIR) -o $@ -c $<

$(NAME):		$(BUILDOBJS)
				$(CC) $(CCFLAG) -I$(INCDIR) -o $(NAME) $(BUILDOBJS)

clean:
				$(RM) $(BUILDOBJS)
				rm -rf $(BUILDDIR)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re
