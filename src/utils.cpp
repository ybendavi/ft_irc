#include "Server.hpp"

std::string	gnm(std::string & buff, std::string & s)
{
	size_t	i;

	i = buff.find("\r\n");
	if ( !(i != std::string::npos) )
		return ("");
	s = buff;
	s.resize(i + 2);
	if (!buff.compare("\r\n"))
		buff.erase();
	else
		buff.erase(0, i + 2);
	return (s);
}

void	sendMessage(User *user, User *receiver, char *domainName)
{

	std::string				to_send;

	//std::cout << "userlooking:" << _users.begin()->first << std::endl;
	//std::cout << "userlooking:" << _users.find(std::string("LS\r\n"))->second.getNickname()<< std::endl;
	//std::cout << "encore ici" << std::endl;
	std::cout << user->getNickname() << std::endl;
	std::cout << receiver->getNickname() << std::endl;
	to_send += user->receivedmsg.front().getToSend();
	//std::cout << "envoyé:" << to_send << std::endl;
	user->receivedmsg.front().setToSend(to_send);

	receiver->tosendmsg.push_back(Message(user->getNickname(), user->getUsername(), user->receivedmsg.front().getToSend().c_str(), std::string(domainName)));
	receiver->getSocket()->events = POLLIN | POLLOUT ;

}

void	sendMessagetochan(User *user, Channel *channel, std::map<std::string, User>::iterator users, std::map<std::string, User>::iterator end, char *domainName)
{
	while (users != end)
	{
		if (channel->isUserOnChannel(users->second.getNickname()) == true)
			sendMessage(user, &(users->second), domainName);
		users++;
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
	str += "\r\n";
	//std::cout << "before send = " << str;
}
