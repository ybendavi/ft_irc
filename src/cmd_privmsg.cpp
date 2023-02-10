/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybendavi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 18:47:17 by ybendavi          #+#    #+#             */
/*   Updated: 2023/02/10 17:32:52 by ybendavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_privMsg(User *user)
{
	std::string					to_send;
	std::map<std::string, Channel>::iterator	it;

	if (user->receivedmsg.front().getParams().empty() == true
		|| user->receivedmsg.front().getParamsopt().empty() == true)
	{
		user->tosendmsg.push_back(Message(ERR_NEEDMOREPARAMS));
		return ;
	}
	if (user->receivedmsg.front().getParams().begin() + 1 == user->receivedmsg.front().getParams().end())
	{

		user->tosendmsg.push_back(Message(ERR_NEEDMOREPARAMS));
		return ;
	}
	if ((user->receivedmsg.front().getParams().begin())->find('#', 0) == 0
		|| (user->receivedmsg.front().getParams().begin())->find('&', 0) == 0)
	{
		it = _channels.find(*(user->receivedmsg.front().getParams().begin()));
		if (it != _channels.end())
		{
			if (it->second.isUserOnChannel(user->getNickname()) == false)
				user->tosendmsg.push_back(Message(ERR_NOSUCHCHANNEL));
			else if (it->second.isUserBan(user->getNickname()) == true)
				user->tosendmsg.push_back(Message(std::string("ERROR You are banned from that channel")));
			else
				sendMessagetochan(user, &(it->second), _users.begin(), _users.end(), _domainName);
		}
		else
			user->tosendmsg.push_back(Message(ERR_CANNOTSENDTOCHAN));
		return ;
	}
	if (_users.find(*(user->receivedmsg.front().getParams().begin())) == _users.end())
	{
		user->tosendmsg.push_back(Message(ERR_NOSUCHNICK));
		return ;
	}
	else
		sendMessage(user, &((_users.find(*(user->receivedmsg.front().getParams().begin())))->second), _domainName);
}
