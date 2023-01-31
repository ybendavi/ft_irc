#ifndef REPLIES_HPP
# define REPLIES_HPP

# define RPL_WELCOME "001 :WELCOME MTHRFCKR \r\n"
# define RPL_YOURHOST "002 \r\n"
# define RPL_CREATED "003 \r\n"
# define RPL_MYINFO "004 \r\n"
# define RPL_PING "PING\r\n"

# define ERR_NICKNAMEINUSE "433"
# define ERR_NONICKNAMEGIVEN "431"
# define ERR_ERRONEUSNICKNAME "432"
# define ERR_NICKCOLLISION "436"
# define ERR_UNAVAILRESOURCE "437"
# define ERR_RESTRICTED "484"
//"<nick> :Nickname is already in use"
# define ERR_NEEDMOREPARAMS "461"
//"<command> :Not enough parameters"
# define ERR_ALREADYREGISTRED "462"
// ":Unauthorized command (already registered)"

#endif
