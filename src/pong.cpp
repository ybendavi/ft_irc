/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybendavi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 18:24:17 by ybendavi          #+#    #+#             */
/*   Updated: 2023/02/09 18:26:01 by ybendavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_pong(User *user)
{
	std::string	pong("PONG \r\n");

	if (user->receivedmsg.front().getParams().empty() == true)
		user->tosendmsg.push_back(Message(ERR_NOORIGIN));
	else
	{
		pong.insert(5, *(user->receivedmsg.front().getParams().begin()));
		user->tosendmsg.push_back(Message(pong.c_str()));
	}
}

