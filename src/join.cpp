/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 17:37:02 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/07 19:22:44 by cdapurif         ###   ########.fr       */
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
        user->tosendmsg.push_back(Message(ERR_NEEDMOREPARAMS + user->getNickname() + " JOIN :Syntax error"));
        return ;
    }
    channelName = (user->receivedmsg.front().getParams())[0];
    if (invalidChannelName(channelName))
    {
        user->tosendmsg.push_back(Message(ERR_NOSUCHCHANNEL + user->getNickname() + " " + channelName + " :No such channel"));
        return ;
    }

    //check if channel exist or create it

    //add user to channel with correct mode
}