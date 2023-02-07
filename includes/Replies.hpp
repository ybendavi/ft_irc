#ifndef REPLIES_HPP
# define REPLIES_HPP

# define RPL_WELCOME "001 :WELCOME MTHRFCKR \r\n"
# define RPL_YOURHOST "002 \r\n"
# define RPL_CREATED "003 \r\n"
# define RPL_MYINFO "004 \r\n"
# define RPL_PING "PING\r\n"

# define RPL_UMODEIS "221 " 
# define ERR_UNKNOWNCOMMAND "421 \r\n"
# define ERR_NOORIGIN "409 \r\n"
# define ERR_NICKNAMEINUSE "433 :Nick in use\r\n"
# define ERR_NOSUCHNICK "401 \r\n"
# define ERR_NONICKNAMEGIVEN "431 :No nickname given\r\n"
# define ERR_ERRONEUSNICKNAME "432"
# define ERR_NICKCOLLISION "436"
# define ERR_UNAVAILRESOURCE "437"
# define ERR_NOTREGISTERED "451 :You have not registered\r\n"

# define ERR_RESTRICTED "484 :Nickname is already in use\r\n"
# define ERR_NEEDMOREPARAMS "461 :Not enough parameters\r\n"
# define ERR_ALREADYREGISTRED "462 :Unauthorized command (already registered)\r\n"
# define ERR_UMODEUNKNOWNFLAG "501 :Unknown MODE flag\r\n"
# define ERR_USERSDONTMATCH "502 :Cannot change mode for other users\r\n"

#endif
