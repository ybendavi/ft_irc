/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 12:08:57 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/09 12:13:09 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::_topic(User *user)
{
    //check parameters
    if (user->receivedmsg.front().getParams().empty())
    {
        user->tosendmsg.push_back(Message(std::string(ERR_NEEDMOREPARAMS) + " TOPIC :Syntax error\r\n"));
        return ;
    }
}