#include "Server.hpp"

void	get_channel_mode(std::map<std::string, Channel>::iterator channel, User *user)
{
	std::string		mode;
	unsigned short	chanmode;

	mode = RPL_CHANNELMODEIS + channel->first + " ";
	chanmode = channel->second.getChannelModes();
	if (chanmode & CHANBAN)
		mode += "b";
	if (chanmode & EXCEPTION)
		mode += "e";
	if (chanmode & LIMIT)
		mode += "l";
	if (chanmode & INVONLY)
		mode += "i";
	if (chanmode & INVEXCEPT)
		mode += "I";
	if (chanmode & KEY)
		mode += "k";
	if (chanmode & MODCHAN)
		mode += "m";
	if (chanmode & SECRET)
		mode += "s";
	if (chanmode & PROTECTOP)
		mode += "t";
	if (chanmode & EXTERNMESS)
		mode += "n";
	user->tosendmsg.push_back(Message(mode));
}

void	Server::get_mode(User * user)
{
	std::string	mode;
	char		umode;

	mode = RPL_UMODEIS;
	mode += ("+");
	umode = user->getMode();
	if (umode & 4)
		mode.push_back('o');
	if (umode & 1)
		mode.push_back('i');
	if (umode & 2)
		mode.push_back('w');
	user->tosendmsg.push_back(Message(mode));
}

void	add_mode(std::string s, User * user)
{
	char	mode;

	mode = user->getMode();
	for (unsigned int i = 1; i < s.size(); ++i)
	{
		if (s[i] == 'i')
			mode |= 1;
		else if (s[i] == 'w')
			mode |= 2;
		else
			user->tosendmsg.push_back(Message(ERR_UMODEUNKNOWNFLAG));
	}
	user->setMode(mode);
}

void	rm_mode(std::string s, User * user)
{
	char	mode;
	
	mode = user->getMode();
	for (unsigned int i = 1; i < s.size(); ++i)
	{
		if (s[i] == 'o' && (mode & 4))
			mode -= 4;
		else if (s[i] == 'i' && (mode & 1))
			mode -= 1;
		else if (s[i] == 'w' && (mode & 2))
			mode -= 2;
		else
			user->tosendmsg.push_back(Message(ERR_UMODEUNKNOWNFLAG));
	}
	user->setMode(mode);
}

void	add_channel_mode(std::string s, std::map<std::string, Channel>::iterator channel)
{
	unsigned short& mode = channel->second.getChannelModes();
	for (unsigned int i = 1; i < s.size(); ++i)
	{
		if (s[i] == 'b')
			mode |= CHANBAN;
		if (s[i] == 'e')
			mode |= EXCEPTION;
		if (s[i] == 'l')
			mode |= LIMIT;
		if (s[i] == 'i')
			mode |= INVONLY;
		if (s[i] == 'I')
			mode |= INVEXCEPT;
		if (s[i] == 'k')
			mode |= KEY;
		if (s[i] == 'm')
			mode |= MODCHAN;
		if (s[i] == 's')
			mode |= SECRET;
		if (s[i] == 't')
			mode |= PROTECTOP;
		if (s[i] == 'n')
			mode |= EXTERNMESS;
	}
}

void	rm_channel_mode(std::string s, std::map<std::string, Channel>::iterator channel)
{
	unsigned short&	mode = channel->second.getChannelModes();

	for (unsigned int i = 1; i < s.size(); ++i)
	{
		if (s[i] == 'b' && (mode & CHANBAN))
			mode -= CHANBAN;
		if (s[i] == 'e' && (mode & EXCEPTION))
			mode -= EXCEPTION;
		if (s[i] == 'l' && (mode & LIMIT))
			mode -= LIMIT;
		if (s[i] == 'i' && (mode & INVONLY))
			mode -= INVONLY;
		if (s[i] == 'I' && (mode & INVEXCEPT))
			mode -= INVEXCEPT;
		if (s[i] == 'k' && (mode & KEY))
			mode -= KEY;
		if (s[i] == 'm' && (mode & MODCHAN))
			mode -= MODCHAN;
		if (s[i] == 's' && (mode & SECRET))
			mode -= SECRET;
		if (s[i] == 't' && (mode & PROTECTOP))
			mode -= PROTECTOP;
		if (s[i] == 'n' && (mode & EXTERNMESS))
			mode -= EXTERNMESS;
	}
}

void	Server::add_channel_user_mode(std::string s, std::map<std::string, Channel>::iterator channel, std::string nickname, User *user, std::string format)
{
	std::string	toSend;

	if (channel->second.isUserOnChannel(nickname) == false)
		return ;

	unsigned char& mode = channel->second.getUserModes(nickname);

	for (unsigned int i = 1; i < s.size(); ++i)
	{
		if (s[i] == 'o')
			mode |= OPERATOR;
		if (s[i] == 'v')
			mode |= VOICE;
		if (s[i] == 'b')
		{
			mode |= BAN;
			std::cout << "banning user " << nickname << " from channel " << channel->first << std::endl;
			//HERE
			toSend = std::string(":") + user->getNickname() + "!~" + user->getUsername() + "@" + "hostname MODE " + channel->first + " +b " + format;
			sendMessageToAllChan(&(channel->second), _users.begin(), _users.end(),toSend);
			channel->second.banUser(nickname);
		}
		if (s[i] == 'i')
			mode |= INVITE;
		if (s[i] == 'm')
			mode |= MODERATED;
		if (s[i] == 't')
			mode |= TOPIC;
	}
}

void	rm_channel_user_mode(std::string s, std::map<std::string, Channel>::iterator channel, std::string nickname)
{
	if (channel->second.isUserOnChannel(nickname) == false && s.size() > 1 && s[1] == 'b')
	{
		std::cout << "unbanning user " << nickname << " from channel " << channel->first << std::endl;
		channel->second.unBanUser(nickname);
	}

	if (channel->second.isUserOnChannel(nickname) == false)
		return ;

	unsigned char&	mode = channel->second.getUserModes(nickname);

	for (unsigned int i = 1; i < s.size(); ++i)
	{
		if (s[i] == 'o' && (mode & OPERATOR))
			mode -= OPERATOR;
		if (s[i] == 'v' && (mode & VOICE))
			mode -= VOICE;
		if (s[i] == 'b' && (mode & BAN))
			mode -= BAN;
		if (s[i] == 'i' && (mode & INVITE))
			mode -= INVITE;
		if (s[i] == 'm' && (mode & MODERATED))
			mode -= MODERATED;
		if (s[i] == 't' && (mode & TOPIC))
			mode -= TOPIC;
	}
}

void	Server::mode_cmd(User * user)
{
	std::vector<std::string>	params;
	
	params = user->receivedmsg.front().getParams();
	if (params.size() < 1)
	{
		user->tosendmsg.push_back(Message(ERR_NEEDMOREPARAMS));
		return ;
	}

	if ((params[0][0] != '#' && params[0][0] != '&') && _users.find(params[0]) == _users.end())
	{
		user->tosendmsg.push_back(Message(ERR_NOSUCHNICK + params[0] + " :No such nick/channel"));
		return ;
	}
	if ((params[0][0] == '#' || params[0][0] == '&') && _channels.find(params[0]) == _channels.end())
	{
		user->tosendmsg.push_back(Message(std::string(ERR_NOSUCHCHANNEL) + params[0] + " :No such channel"));
		return ;
	}
	
	//if parameter is a user
	if (_users.find(params[0]) != _users.end())
	{
		if (user->getNickname() != params[0])
		{
			user->tosendmsg.push_back(Message(ERR_USERSDONTMATCH));
			return ;
		}

		if (params.size() == 1)
		{
			get_mode(user);
			return ;
		}
		std::vector<std::string>::iterator	it;
		it = params.begin();
		++it;
		while (it != params.end())
		{
			std::string s = *it;
			if (s[0] == '+')
				add_mode(s, user);
			else if (s[0] == '-')
				rm_mode(s, user);
			else
			{
				user->tosendmsg.push_back(Message(ERR_UMODEUNKNOWNFLAG));
				return ;
			}
			it++;
		}
		get_mode(user);
	}
	else
	{
		if (params.size() == 1)
		{
			get_channel_mode(_channels.find(params[0]), user);
			return ;
		}

		std::map<std::string, Channel>::iterator chan = _channels.find(params[0]);
		if (!(chan->second.getUserModes(user->getNickname()) & OPERATOR))
			user->tosendmsg.push_back(Message(std::string(ERR_CHANOPRIVSNEEDED) + params[0] + " :You're not channel operator"));

		if (params.size() == 2)
		{
			std::string s = params[1];

			if (s[0] == '+')
				add_channel_mode(s, chan);
			else if (s[0] == '-')
				rm_channel_mode(s, chan);
			else
			{
				user->tosendmsg.push_back(Message(ERR_UMODEUNKNOWNFLAG));
				return ;
			}
			get_channel_mode(chan, user);
		}
		if (params.size() == 3)
		{
			std::string nickname;

			if (params[2].size() > 3 && params[2][1] == '!')
			{
				nickname = params[2].substr(3);
				nickname = nickname.substr(0, nickname.find_last_of('@'));
				nickname = _findUserByUsername(nickname);
			}
			else
				nickname = params[2];
			if (chan->second.isUserOnChannel(nickname) == false && chan->second.isUserBan(nickname) == false)
			{
	        	user->tosendmsg.push_back(Message(std::string(ERR_USERNOTINCHANNEL) + params[0] + " :They aren't on that channel"));
				return ;
    		}

			std::string s = params[1];

			if (s[0] == '+')
				add_channel_user_mode(s, chan, nickname, user, params[2]);
			else if (s[0] == '-')
				rm_channel_user_mode(s, chan, nickname);
			else
			{
				user->tosendmsg.push_back(Message(ERR_UMODEUNKNOWNFLAG));
				return ;
			}
			//get_user_on_channel_mode
		}
	}
}
