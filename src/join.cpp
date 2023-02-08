/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 17:37:02 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/08 15:28:03 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Replies.hpp"
#include "Message.hpp"

bool    invalidChannelName(const std::string& channelName)
{
    if (channelName[0] != '#' && channelName[0] != '&')
        return (true);
    if (channelName.find_first_of(" ,") != std::string::npos || channelName.find('^G') != std::string::npos)
        return (true);
    return (false);
}

void	Server::_join(User *user)
{
    std::string to_send;
    std::string channelName;

	//check parameters
    if (user->receivedmsg.front().getParams().empty() == true)
    {
        user->tosendmsg.push_back(Message(ERR_NEEDMOREPARAMS + user->getNickname() + " JOIN :Syntax error\r\n"));
        return ;
    }
    channelName = (user->receivedmsg.front().getParams())[0];
    if (invalidChannelName(channelName))
    {
        user->tosendmsg.push_back(Message(ERR_NOSUCHCHANNEL + user->getNickname() + " " + channelName + " :No such channel\r\n"));
        return ;
    }

    //check if channel exist or create it
    std::map<std::string, Channel>::iterator    chan = _channels.find(channelName);
    if (chan == _channels.end())
    {
        _channels[channelName] = Channel(channelName);
        _channels[channelName].addUser(user->getNickname(), OPERATOR | VOICE | INVITE | TOPIC);
    }
    else
        chan->second.addUser(user->getNickname());
}