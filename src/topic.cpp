/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 12:08:57 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/09 16:57:02 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::_topic(User *user)
{
    //std::string to_send; will be useful for stocking successful message and sending it to every users on that channel
    std::string channelName;

    //no channel name given
    if (user->receivedmsg.front().getParams().empty())
    {
        user->tosendmsg.push_back(Message(std::string(ERR_NEEDMOREPARAMS) + " TOPIC :Syntax error"));
        return ;
    }

    channelName = (user->receivedmsg.front().getParams())[0];
    std::map<std::string, Channel>::iterator    chan = _channels.find(channelName);
    
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
    
    //no topic specified
    if (user->receivedmsg.front().getParamsopt().empty() && user->receivedmsg.front().getColon() == false) //maybe use boolean here to check if paramopt was never set
    {
        if (chan->second.getTopic().empty())
            user->tosendmsg.push_back(Message(std::string(RPL_NOTOPIC) + channelName + " :No topic is set"));
        else
        {
            user->tosendmsg.push_back(Message(std::string(RPL_TOPIC) + channelName + " :" + chan->second.getTopic()));
            user->tosendmsg.push_back(Message(std::string(RPL_TOPICWHOTIME) + channelName + " " + chan->second.getTopicCreator()));
        }
        return ;
    }

    //set topic
    if (chan->second.getChannelModes() & PROTECTOP && !(chan->second.getUserModes(user->getNickname()) & OPERATOR)) //if topic is protected and user is not operator
        user->tosendmsg.push_back(Message(std::string(ERR_CHANOPRIVSNEEDED) + channelName + " :You're not channel operator"));
    else
    {
        chan->second.getTopic() = user->receivedmsg.front().getParamsopt();
        chan->second.getTopicCreator() = user->getNickname();
        user->tosendmsg.push_back(Message(std::string(":") + user->getNickname() + "!~" + user->getUsername() + "@" + "hostname TOPIC " + channelName + " :" + chan->second.getTopic()));
    }
}