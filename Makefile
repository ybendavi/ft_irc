NAME	= ircserv

SRCS	= Server.cpp main.cpp User.cpp Message.cpp

CC		= c++

OBJS	= $(SRCS:.cpp=.o)

DEPS	= $(OBJS:.o=.d)

CFLAGS	= -Wall -Wextra -Werror -MMD -std=c++98 -g3

IFLAGS	= #put includes -Iincludes here

all		:	$(NAME)

%.o		:	%.cpp
			$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(NAME)	:	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) $(IFLAGS) -o $@

clean	:
			rm -rf $(DEPS)
			rm -rf $(OBJS)

fclean	:	clean
			rm -rf $(NAME)

re		:	fclean
			make

-include $(DEPS)

.PHONY	:	all fclean clean re
