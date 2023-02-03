#include "Server.hpp"

//probably need some modifications because of params numbers being wird w/ irssi
//also need to check for err need more params and how to put properly message into it

std::string	Server::cmd_user(Message & msg, User & user)
{
	if ( !(user.getUsername().empty()) )
		return (ERR_ALREADYREGISTRED);

	user.setUsername(msg.getParams()[0]);
	user.setIp(msg.getParams()[2]);
	user.setRealname(msg.getParamsopt());
	++_nbUsers;
	_users[user.getNickname()].tosendmsg.push_back(Message(RPL_WELCOME));
	_users[user.getNickname()].tosendmsg.push_back(Message(RPL_YOURHOST));
	_users[user.getNickname()].tosendmsg.push_back(Message(RPL_CREATED));
	_users[user.getNickname()].tosendmsg.push_back(Message(RPL_MYINFO));
	return ("");
}
