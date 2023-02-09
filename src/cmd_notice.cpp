/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_notice.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybendavi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 18:46:49 by ybendavi          #+#    #+#             */
/*   Updated: 2023/02/09 18:50:04 by ybendavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_notice(User *user)
{
	std::string					to_send;
	std::map<std::string, Channel>::iterator	it;

	if (user->receivedmsg.front().getParams().empty() == true
		|| user->receivedmsg.front().getParamsopt().empty() == true)
		return ;
	if (user->receivedmsg.front().getParams().begin() + 1 == user->receivedmsg.front().getParams().end())
		return ;
	if ((user->receivedmsg.front().getParams().begin())->find('#', 0) == 0
		|| (user->receivedmsg.front().getParams().begin())->find('&', 0) == 0)
	{
		it = _channels.find(*(user->receivedmsg.front().getParams().begin()));

		if (it != _channels.end())
		{
			if (it->second.isUserOnChannel(user->getNickname()) == true)
				sendMessagetochan(user, &(it->second), _users.begin(), _users.end(), _domainName);
		}
		return ;
	}
	if (_users.find(*(user->receivedmsg.front().getParams().begin())) == _users.end())
		return ;
	else
		sendMessage(user, &((_users.find(*(user->receivedmsg.front().getParams().begin())))->second), _domainName);
}
