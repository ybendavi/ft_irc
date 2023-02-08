/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 20:14:44 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/08 17:27:43 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, unsigned short modes) : _channel(name), _chanMode(modes), _chanUsers()
{
    std::cout << _chanMode << std::endl; //temporary
}

Channel::~Channel(void)
{}

void    Channel::removeUserFromChannel(const std::string& nickname)
{
    _chanUsers.erase(nickname);
}

int    Channel::_checkUserPerms(const std::string& nickname)
{
    std::map<std::string, unsigned char>::iterator  it = _chanUsers.find(nickname);

    if (it == _chanUsers.end())
        return (0);
    if (_chanMode & CHANBAN)
    {
        if (it->second & BAN)
            return (1); //1 means user is blocked in this channel
    }
    return (0);
}

void    Channel::addUser(const std::string& user, unsigned char modes)
{
    int status;

    status = _checkUserPerms(user);
    if (status == 0)
        _chanUsers[user] = modes;
    else
    {
        if (status == 1)
            std::cout << "user " << user << " is banned from channel " << _channel << std::endl;
        else
            std::cout << "special case when adding user " << user << " to channel " << _channel << std::endl;
    }
}