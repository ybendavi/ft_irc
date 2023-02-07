#include "Server.hpp"

//probably need some modifications because of params numbers being wird w/ irssi
//also need to check for err need more params and how to put properly message into it

void	Server::cmd_user(User * user)
{
	if ( !(user->getUsername().empty()) )
	{
		user->tosendmsg.push_back(Message(ERR_ALREADYREGISTRED));
		return ;
	}
	if (user->receivedmsg.front().getParams().size() < 3
			|| user->receivedmsg.front().getParamsopt().empty() )
	{
		user->tosendmsg.push_back(Message(ERR_NEEDMOREPARAMS));
		return ;
	}
	user->setUsername(user->receivedmsg.front().getParams()[0]);
	user->setIp(user->receivedmsg.front().getParams()[2]);
	user->setRealname(user->receivedmsg.front().getParamsopt());
	_users[user->getNickname()].tosendmsg.push_back(Message(RPL_WELCOME));
	_users[user->getNickname()].tosendmsg.push_back(Message(RPL_YOURHOST));
	_users[user->getNickname()].tosendmsg.push_back(Message(RPL_CREATED));
	_users[user->getNickname()].tosendmsg.push_back(Message(RPL_MYINFO));
	++_nbUsers;
}
