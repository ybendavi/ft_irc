/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 18:30:30 by ccottin           #+#    #+#             */
/*   Updated: 2023/02/01 23:49:53 by ybendavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include <iostream>

User::User(void) : _isOperator(false), _isInvisible(false),
					_isWallopable(true), _socket(NULL), _addr(NULL)
{ }

User::User(struct pollfd * socket, struct sockaddr * addr) : _isOperator(false), 
					_isInvisible(false), _isWallopable(true),
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
		this->_isOperator = ref.getOp();
		this->_isInvisible = ref.getInv();
		this->_isWallopable = ref.getWal();
		this->_realname = ref.getRealname();
		this->_nickname = ref.getNickname();
		this->_pass = ref.getPass();
		this->_ip = ref.getIp();
		this->_socket = ref.getSocket();
		this->_addr = ref.getAddr();
	}
	return (*this);
}

bool		User::getOp(void) const { return (this->_isOperator); } 

bool		User::getInv(void) const { return (this->_isInvisible); }

bool		User::getWal(void) const { return (this->_isWallopable); }

std::string	User::getRealname(void) const { return (this->_realname); }

std::string	User::getUsername(void) const { return (this->_username); }

std::string	User::getNickname(void) const { return (this->_nickname); }
		
std::string	User::getPass(void) const { return (this->_pass); }

std::string	User::getIp(void) const { return (this->_ip); }

struct pollfd	*User::getSocket(void) { return (this->_socket); }
 
struct pollfd	*User::getSocket(void) const { return (this->_socket); }

struct sockaddr	*User::getAddr(void) { return (this->_addr); }
 
struct sockaddr	*User::getAddr(void) const { return (this->_addr); }


//struct pollfd	User::getSocket(void) const { return (this->_socket); }

void		User::setOp(bool b) 
{ 
	if (_isOperator != b)
		_isOperator = b;
}

void		User::setIp(std::string s) 
{ 
	if (_ip != s)
		_ip = s;
}

void		User::setInv(bool b)
{ 
	if (_isInvisible != b)
		_isInvisible = b;
}

void		User::setWal(bool b)
{ 
	if (_isWallopable != b)
		_isWallopable = b;
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

void		User::setPass(std::string s)
{
	if (_pass != s)
		_pass = s;
}
/*
std::string		User::_checkParam(void)
{
	if (_nickname.size() > 9)
		return (
}*/

void	User::parseUser(char * buffer)
{
	std::string	s(buffer);
	int	i;
	int	y;

	i = s.find('\r') + 1;
	std::string	tmp(buffer, 0, i + 1);
	i++;
	i = s.find(' ', i) + 1;
	y = s.find('\r', i);
	this->_nickname = s.substr(i, y - i);
	i = y + 2;
	i = s.find(' ', i) + 1;
	y = s.find(' ', i);
	this->_username = s.substr(i, y - i);
	i = s.find(' ', y + 1) + 1;
	y = s.find(' ', i);
	this->_ip = s.substr(i, y - i);
	i = s.find(':', y) + 1;
	y = s.find('\r', i);
	this->_realname = s.substr(i, y - i);
	//return (checkParams());
	//return (RPL_WELCOME);
}

