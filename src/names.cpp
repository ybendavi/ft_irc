/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 18:33:56 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/09 19:21:06 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::_listAllChannels(User *user)
{
    std::map<std::string, Channel>::iterator    it = _channels.begin();

    for (; it != _channels.end(); it++)
        user->tosendmsg.push_back(Message(it->second.listUsersOnChannel()));
}

void    Server::_names(User *user)
{
    std::string channelName;

    //no channel name given
    if (user->receivedmsg.front().getParams().empty())
    {
        _listAllChannels(user);
        user->tosendmsg.push_back(Message(std::string(RPL_ENDOFNAMES) + "* :End of NAMES list"));
    }
    else
    {
        channelName = (user->receivedmsg.front().getParams())[0];
        std::map<std::string, Channel>::iterator chan = _channels.find(channelName);

        if (chan != _channels.end())
            user->tosendmsg.push_back(Message(chan->second.listUsersOnChannel()));
        user->tosendmsg.push_back(Message(std::string(RPL_ENDOFNAMES) + channelName + " :End of NAMES list"));
    }
}