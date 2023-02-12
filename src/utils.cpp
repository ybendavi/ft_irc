#include "Server.hpp"

std::string	gnm(std::string & buff, std::string & s)
{
	size_t	i;

	i = buff.find("\r\n");
	if ( !(i != std::string::npos) )
		return ("");
	s = buff;
	s.resize(i + 2);
	buff.erase(0, i + 2);
	if (!s.compare("\r\n"))
		return ("");
	/*if (!buff.compare("\r\n"))
		buff.erase();
	else
		buff.erase(0, i + 2);*/
	return (s);
}

void	sendMessage(User *user, User *receiver, char *domainName)
{

	std::string				to_send;

	to_send += user->receivedmsg.front().getToSend();
	user->receivedmsg.front().setToSend(to_send);

	receiver->tosendmsg.push_back(Message(user->getNickname(), user->getUsername(), user->receivedmsg.front().getToSend().c_str(), std::string(domainName)));
	receiver->getSocket()->events = POLLIN | POLLOUT ;

}

void	sendMessagetochan(User *user, Channel *channel, std::map<std::string, User>::iterator users, std::map<std::string, User>::iterator end, char *domainName)
{
	while (users != end)
	{
		if (channel->isUserOnChannel(users->first) && &(users->second) != user)
			sendMessage(user, &(users->second), domainName);
		users++;
	}
}

void	sendMessageToAllChan(Channel *channel, std::map<std::string, User>::iterator users, std::map<std::string, User>::iterator end, std::string message)
{
	for (; users != end; ++users)
	{
		if (channel->isUserOnChannel(users->first))
		{
			users->second.tosendmsg.push_back(Message(message));
			users->second.getSocket()->events = POLLIN | POLLOUT ;
		}
	}
}


void	readySendy(std::string &str, std::string domain, std::string nick)
{
	if (str[0] != ':' && str.compare(0, 3, "PONG", 0, 3))
	{
		std::string temp = " ";

		temp += nick;
		str.insert(3, temp);
		temp.erase();
		temp = ":";
		temp += domain + " ";
		str.insert(0, temp);
	}
	if (str.find_last_of("\r\n") == std::string::npos)
		str += "\r\n";
}
