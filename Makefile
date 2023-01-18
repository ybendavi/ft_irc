NAME	= ircserv

SRCS	= 

CC		= c++

DEPS	= $(OBJS:.o=.d)

CFLAGS	= -Wall -Wextra - Werror -std=c++98 -MMD -g3

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
