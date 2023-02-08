#ifndef REPLIES_HPP
# define REPLIES_HPP

# define RPL_WELCOME "001 :WELCOME MTHRFCKR "
# define RPL_YOURHOST "002 "
# define RPL_CREATED "003 "
# define RPL_MYINFO "004 "
# define RPL_PING "PING"

# define RPL_UMODEIS "221 " 
# define ERR_UNKNOWNCOMMAND "421 "
# define ERR_NOORIGIN "409 "
# define ERR_NICKNAMEINUSE "433 :Nick in use"
# define ERR_NOSUCHNICK "401 "
# define ERR_NOSUCHCHANNEL "403 "
# define ERR_NONICKNAMEGIVEN "431 :No nickname given"
# define ERR_ERRONEUSNICKNAME "432"
# define ERR_NICKCOLLISION "436"
# define ERR_UNAVAILRESOURCE "437"
# define ERR_NOTONCHANNEL "442 "
# define ERR_NOTREGISTERED "451 :You have not registered"

# define ERR_RESTRICTED "484 :Nickname is already in use"
# define ERR_NEEDMOREPARAMS "461 :Not enough parameters"
# define ERR_ALREADYREGISTRED "462 :Unauthorized command (already registered)"
# define ERR_UMODEUNKNOWNFLAG "501 :Unknown MODE flag"
# define ERR_USERSDONTMATCH "502 :Cannot change mode for other users"

#endif
