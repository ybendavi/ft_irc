/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 10:42:32 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/10 14:43:27 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::_kick(User *user)
{
    std::string channelName;
    std::string targetName;
    std::string reason;
    std::string toSend;

    //no channel name and/or no nickname given
    if (user->receivedmsg.front().getParams().size() < 2)
    {
        user->tosendmsg.push_back(Message(std::string(ERR_NEEDMOREPARAMS) + " KICK :Syntax error"));
        return ;
    }

    channelName = (user->receivedmsg.front().getParams())[0];
    std::map<std::string, Channel>::iterator chan = _channels.find(channelName);
    
    //no such channel
    if (chan == _channels.end())
    {
        user->tosendmsg.push_back(Message(std::string(ERR_NOSUCHCHANNEL) + channelName + " :No such channel"));
        return ;
    }

    //check if user is on channel
    if (chan->second.isUserOnChannel(user->getNickname()) == false)
    {
        user->tosendmsg.push_back(Message(std::string(ERR_NOTONCHANNEL) + channelName + " :You're not on that channel"));
        return ;
    }

    //check if target is on channel
    targetName = (user->receivedmsg.front().getParams())[1];
    if (chan->second.isUserOnChannel(targetName) == false)
    {
        user->tosendmsg.push_back(Message(std::string(ERR_USERNOTINCHANNEL) + channelName + " :They aren't on that channel"));
        return ;
    }

    //check if user is operator
    if (!(chan->second.getUserModes(user->getNickname()) & OPERATOR))
        user->tosendmsg.push_back(Message(std::string(ERR_CHANOPRIVSNEEDED) + channelName + " :You're not channel operator"));

    reason = user->receivedmsg.front().getParamsopt();
    if (reason.empty())
        reason = user->getNickname();
    toSend = ":" + user->getNickname() + "!~" + user->getUsername() + "@hostname KICK " + channelName + " " + targetName + " :" + reason;
    sendMessageToAllChan(&(chan->second), _users.begin(), _users.end(),toSend);
    chan->second.removeUserFromChannel(targetName);
}