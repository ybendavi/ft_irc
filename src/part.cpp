/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 20:23:55 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/13 19:30:20 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::_part(User *user)
{
    std::string toSend;
    std::string channelName;
    std::string leaveMessage;

    //check parameters
    if (user->receivedmsg.front().getParams().empty())
    {
        user->tosendmsg.push_back(Message(std::string(ERR_NEEDMOREPARAMS) + " PART :Syntax error"));
        return ;
    }
    if (user->receivedmsg.front().getParamsopt().empty() == false)
        leaveMessage = user->receivedmsg.front().getParamsopt();

    channelName = (user->receivedmsg.front().getParams())[0];
    if (invalidChannelName(channelName))
    {
        std::cout << "Invalid channel name" << std::endl;
        user->tosendmsg.push_back(Message(std::string(ERR_NOSUCHCHANNEL) + " " + channelName + " :No such channel"));
        return ;
    }

    //check if channel exist and if user is on it
    std::map<std::string, Channel>::iterator    chan = _channels.find(channelName);
    if (chan == _channels.end())
        user->tosendmsg.push_back(Message(std::string(ERR_NOSUCHCHANNEL) + " " + channelName + " :No such channel"));
    else
    {
        if (chan->second.isUserOnChannel(user->getNickname()))
        {
            toSend = std::string(":") + user->getNickname() + "!~" + user->getUsername() + "@" + "hostname PART " + channelName + " :" + leaveMessage;
            sendMessageToAllChan(&(chan->second), _users.begin(), _users.end(),toSend);
            chan->second.removeUserFromChannel(user->getNickname());
            if (chan->second.size() == 0)
            {
                std::cout << "last user leaving channel, destroying " << channelName << std::endl;
                _channels.erase(chan);
            }
        }
        else
            user->tosendmsg.push_back(Message(std::string(ERR_NOTONCHANNEL) + channelName + " :You're not on that channel"));
    }
}