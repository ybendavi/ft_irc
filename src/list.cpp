/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 21:10:32 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/09 21:11:57 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::_list(User *user)
{
    //no channel name given
    if (user->receivedmsg.front().getParams().empty())
    {
        //_listAllChannels(user);
        //user->tosendmsg.push_back(Message(std::string(RPL_ENDOFNAMES) + "* :End of NAMES list"));
    }
}