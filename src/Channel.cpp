/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 20:14:44 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/08 21:31:33 by ybendavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, unsigned short modes) : _channel(name), _chanMode(modes), _chanUsers()
{
    std::cout << _chanMode << std::endl; //temporary
}

Channel::~Channel(void)
{}

void    Channel::addUser(const std::string& user, unsigned char modes)
{
    _chanUsers[user] = modes;
}

bool    Channel::isUserOnChannel(const std::string& nickname)
{
    if (_chanUsers.find(nickname) == _chanUsers.end())
        return (false);
    return (true);
}
