/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 21:10:32 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/09 22:13:38 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::_channelsList(User *user)
{
    std::map<std::string, Channel>::iterator    it = _channels.begin();

    for (; it != _channels.end(); it++)
        user->tosendmsg.push_back(Message(it->second.listChannelsInfo()));
}

void    Server::_list(User *user)
{
    std::string channelName;

    user->tosendmsg.push_back(Message(std::string(RPL_LISTSTART) + "Channel :Users  Name"));

    //no channel name given
    if (user->receivedmsg.front().getParams().empty())
        _channelsList(user);
    else
    {
        channelName = (user->receivedmsg.front().getParams())[0];
        std::map<std::string, Channel>::iterator chan = _channels.find(channelName);

        if (chan != _channels.end())
            user->tosendmsg.push_back(Message(chan->second.listChannelsInfo()));
    }
    user->tosendmsg.push_back(Message(std::string(RPL_LISTEND) + ":End of /LIST"));
}