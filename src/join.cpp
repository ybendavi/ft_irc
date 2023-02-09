/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 17:37:02 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/09 17:02:09 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool    invalidChannelName(const std::string& channelName)
{
    if (channelName[0] != '#' && channelName[0] != '&')
        return (true);
    if (channelName.find_first_of(" ,") != std::string::npos || channelName.find(7) != std::string::npos)
        return (true);

    return (false);
}

void	Server::_join(User *user)
{
    //std::string to_send; //might be useful for formatting responses
    std::string channelName;

	//check parameters
    if (user->receivedmsg.front().getParams().empty())
    {
        user->tosendmsg.push_back(Message(std::string(ERR_NEEDMOREPARAMS) + " JOIN :Syntax error"));
        return ;
    }
    channelName = (user->receivedmsg.front().getParams())[0];
    if (invalidChannelName(channelName))
    {
        std::cout << "Invalid channel name" << std::endl;
        user->tosendmsg.push_back(Message(std::string(ERR_NOSUCHCHANNEL) + " " + channelName + " :No such channel"));
        return ;
    }

    std::cout << "user " << user->getNickname() << " wants to join/create channel " << channelName << std::endl;

    //check if channel exist or create it
    std::map<std::string, Channel>::iterator    chan = _channels.find(channelName);
    if (chan == _channels.end())
    {
        std::cout << "channel " << channelName << " didn't exist, so " << user->getNickname() << " creates it join as operator" << std::endl;
        //if (user->receivedmsg.front().getParamsopt().empty()) //option might be useful to create channl with specific modes
        _channels.insert(std::make_pair(channelName, Channel(channelName)));
        /*else
            _channels.insert(std::make_pair(channelName, Channel(channelName, function_to_give_mode() )));*/
        chan = _channels.find(channelName);
        chan->second.addUser(user->getNickname(), OPERATOR | VOICE | INVITE | TOPIC);
    }
    else
    {
        //check user modes first
        //HERE
        std::cout << user->getNickname() << " join channel " << channelName << " as normal user" << std::endl;
        chan->second.addUser(user->getNickname());
    }
    user->tosendmsg.push_back(Message(std::string(":") + user->getNickname() + "!~" + user->getUsername() + "@" + "hostname JOIN :" + channelName));
    if (chan->second.getTopic().empty() == false)
        user->tosendmsg.push_back(Message(std::string(RPL_TOPIC) + channelName + " :" + chan->second.getTopic()));
}