/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 18:30:30 by ccottin           #+#    #+#             */
/*   Updated: 2023/02/09 22:36:44 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include <iostream>

User::User(void) : _mode(3), _socket(NULL), _addr(NULL)
{ }

User::User(struct pollfd * socket, struct sockaddr * addr) : _mode(3), 
					_socket(socket), _addr(addr)
{ }

User::User(const User &ref) : _socket(ref.getSocket()), _addr(ref.getAddr())
{ 
	*this = ref;
}

User::~User(void)
{ 
	//close(_socket);
}

User	&User::operator=(const User &ref)
{
	if (this != &ref)
	{
		this->_mode = ref.getMode();
		this->_realname = ref.getRealname();
		this->_username = ref.getUsername();
		this->_nickname = ref.getNickname();
		this->_ip = ref.getIp();
		this->_socket = ref.getSocket();
		this->_addr = ref.getAddr();
		this->receivedmsg = ref.receivedmsg;
		this->tosendmsg = ref.tosendmsg;
	}
	return (*this);
}

std::string	User::getRealname(void) const { return (this->_realname); }

std::string	User::getUsername(void) const { return (this->_username); }

std::string	User::getNickname(void) const { return (this->_nickname); }
		
std::string	User::getIp(void) const { return (this->_ip); }

struct pollfd	*User::getSocket(void) { return (this->_socket); }
 
struct pollfd	*User::getSocket(void) const { return (this->_socket); }

struct sockaddr	*User::getAddr(void) { return (this->_addr); }
 
struct sockaddr	*User::getAddr(void) const { return (this->_addr); }

char			User::getMode(void) const { return (this->_mode); }

//struct pollfd	User::getSocket(void) const { return (this->_socket); }

void		User::setEvent(short i)
{
	_socket->events = i;
}
void		User::setIp(std::string s) 
{ 
	if (_ip != s)
		_ip = s;
}

void		User::setMode(char m)
{
	if (_mode != m)
		_mode = m;
}

void		User::setRealname(std::string s)
{
	if (_realname != s)
		_realname = s;
}

void		User::setNickname(std::string s)
{
	if (_nickname != s)
		_nickname = s;
}

void		User::setUsername(std::string s)
{
	if (_username != s)
		_username = s;
}
