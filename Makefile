NAME	= ircserv

CXX		= c++

CXXFLAGS	= -Wall -Wextra -Werror -MMD -std=c++98 -g3

SRCS	=	Server.cpp \
			main.cpp \
			User.cpp \
			Message.cpp \
			utils.cpp

OBJS	= $(addprefix src/, $(SRCS:.cpp=.o))

DEPS	= $(OBJS:.o=.d)

IFLAGS	= -Iincludes

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			$(CXX) $(CXXFLAGS) $(IFLAGS) -o $@ $^

%.o		:	%.cpp
			$(CXX) $(CXXFLAGS) $(IFLAGS) -c $< -o $@

clean	:
			$(RM) $(DEPS)
			$(RM) $(OBJS)

fclean	:	clean
			$(RM) $(NAME)

re		:	fclean all

-include $(DEPS)

.PHONY	:	all fclean clean re