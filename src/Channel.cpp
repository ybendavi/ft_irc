/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 20:14:44 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/10 18:15:07 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, unsigned short modes) : _channel(name), _chanMode(modes), _chanUsers(), _banList(), _chanLimit(-1)
{
    (void)_chanLimit;
}

Channel::~Channel(void)
{}

bool    Channel::isUserOnChannel(const std::string& nickname)
{
    if (_chanUsers.find(nickname) == _chanUsers.end())
        return (false);
    return (true);
}

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
}

unsigned short  Channel::size()
{
    return (_chanUsers.size());
}

std::string&    Channel::getTopic()
{
    return (_topic[0]);
}

std::string&    Channel::getTopicCreator()
{
    return (_topic[1]);
}

unsigned short& Channel::getChannelModes(void)
{
    return (_chanMode);
}

unsigned char&  Channel::getUserModes(const std::string& nickname)
{
    return (_chanUsers[nickname]);
}

std::string Channel::listUsersOnChannel()
{
    std::string toSend = std::string(RPL_NAMREPLY) + "= " + _channel + " :";
    std::map<std::string, unsigned char>::iterator  it = _chanUsers.begin();

    for (; it != _chanUsers.end(); it++)
    {
        if (it->second & OPERATOR)
            toSend += "@" + it->first;
        else
            toSend += it->first;
        toSend += " ";
    }
    return (toSend);
}

std::string Channel::listChannelsInfo()
{
    std::string         number; 
    std::stringstream   ss;

    ss << size();
    ss >> number;

    return (std::string(RPL_LIST) + _channel + " " + number + " :" + _topic[0]);
}

void    Channel::banUser(std::string nickname)
{
    _banList.push_back(nickname);
    removeUserFromChannel(nickname);
}

void    Channel::unBanUser(std::string nickname)
{
    for (std::vector<std::string>::iterator it = _banList.begin(); it != _banList.end(); ++it)
    {
        if (*it == nickname)
        {
            _banList.erase(it);
            break ;
        }
    }
}

bool    Channel::isUserBan(std::string nickname)
{
    for (std::vector<std::string>::iterator it = _banList.begin(); it != _banList.end(); ++it)
    {
        if (*it == nickname)
            return (true);
    }
    return (false);
}