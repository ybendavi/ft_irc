/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 20:51:06 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/07 21:11:12 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::_quit(User *user)
{
    std::string to_send;

    to_send += ": " + user->getNickname() + "!" + user->getRealname();

    if (!(user->receivedmsg.front().getParams().empty()))
        to_send += " " + (user->receivedmsg.front().getParams())[0];

    user->tosendmsg.push_back(Message(to_send)); //Pas sur de comment utiliser tout ca encore, mais user doit envoyer ce message (si correctement formate) avant l'appel a disconnectClient

    _disconnectClient(*(user->getSocket()));
}