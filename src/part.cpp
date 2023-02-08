/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 20:23:55 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/08 21:00:01 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::_part(User *user)
{
    std::string to_send;
    std::string channelName;

    //check parameters
    if (user->receivedmsg.front().getParams().empty())
    {
        user->tosendmsg.push_back(Message(std::string(ERR_NEEDMOREPARAMS) + " PART :Syntax error\r\n"));
        return ;
    }
    channelName = (user->receivedmsg.front().getParams())[0];
    std::cout << "User " << user->getNickname() << " wants to leave channel " << channelName << std::endl;
    if (invalidChannelName(channelName))
    {
        std::cout << "Invalid channel name" << std::endl;
        user->tosendmsg.push_back(Message(std::string(ERR_NOSUCHCHANNEL) + " " + channelName + " :No such channel\r\n"));
        return ;
    }

    //check if channel exist and if user is on it
    std::map<std::string, Channel>::iterator    chan = _channels.find(channelName);
    if (chan == _channels.end())
        user->tosendmsg.push_back(Message(std::string(ERR_NOSUCHCHANNEL) + " " + channelName + " :No such channel\r\n"));
    else
    {
        if (chan->second.isUserOnChannel(user->getNickname()))
            chan->second.removeUserFromChannel(user->getNickname());
        else
            user->tosendmsg.push_back(Message(std::string(ERR_NOTONCHANNEL) + channelName + " :You're not on that channel\r\n"));
    }
}