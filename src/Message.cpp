/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 18:28:25 by ccottin           #+#    #+#             */
/*   Updated: 2023/02/01 19:55:50 by ybendavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <iostream>

//Message::Message(void)
//{ }

//Message::Message(const Message &ref)
//{ 
//	*this = ref;
//}

Message::~Message(void)
{ } 

Message::Message(const char *message )
{ 
	std::vector<std::string>::iterator it;

	_tosend = std::string(message);
	parseMessage(message);
	std::cout << "Message:" << message << std::endl << "Receiver:" << _receiver << std::endl
		<< "Command:" << _command << std::endl
		<< "Params:";
	it = _params.begin();
	while (it != _params.end())
	{
		std::cout << *it << " ";
		it++;
	}
	std::cout << std::endl;
	std::cout << "ParamsOptional:" << _paramsopt << std::endl;
}

Message	&Message::operator=(const Message &ref)
{
	if (this != &ref)
	{
		this->_receiver = ref._receiver;
		this->_command = ref._command;
	}
	return (*this);
}

std::string	Message::getToSend(void) const { return (_tosend); }

std::string	Message::getParamsopt(void) const { return (_paramsopt); }

std::string	Message::getMessage(void) const { return (_message); }

std::string	Message::getCommand(void) const { return (_command); }

std::vector<std::string>	Message::getParams(void) const { return (_params); }

int	Message::parseMessage(const std::string message)
{
	size_t		pos;
	size_t		param_pos;
	size_t		last_pos;
	std::string	tmp;
	int		count;

	count = 0;
	if (message.empty() == true || message.size() > 512)
		return (-1);
	last_pos = message.find_first_of('\r');
	if (last_pos == std::string::npos || message[last_pos + 1] != '\n')
	{
		std::cout << "retour chariot" << std::endl;
		return (-1);
	}
//	PREFIX		//

	//if (message.find_first_of(':') == 0)
	//	message.erase(0, message.find_first_of(' ') + 1);
//	CMDS		//
	if (message.find_first_of("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm") != 0 && message.find_first_of("01234567890") != 0)
	{
		std::cout << "Pas les bons char" << std::endl;
		return (-1);
	}
	else
	{
		if (message.find_first_of("0123456789") == 0 && message.find_first_not_of("0123456789") != 3)
			return (-1);
	}
	std::cout << "1" << std::endl;
//	PARAMS		//
	if (message.find_first_of("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm") == 0)
		pos = message.find_first_not_of("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm");
	else
		pos = message.find_first_not_of("0123456789");
	if (pos <= last_pos)
		_command = message.substr(0, pos);
	else
	{
		std::cout << "pos < " << std::endl;
		return (-1);
	}
	param_pos = pos;
	if (message[pos] != ' ')
		return (-1);
	while ((param_pos = message.find_first_of(' ', param_pos)) != std::string::npos && message[param_pos + 1] != ':')
	{
		param_pos++;
		count++;
	}
	if (count > 14)
		return (-1);	
	if (message[pos] == ' ')
		pos++;
//	RECEIVER	//
	param_pos = pos;
	while ( pos != std::string::npos && pos != message.find_first_of("\r\n:"))
	{
		param_pos = message.find_first_of(" \r\n:", param_pos);
		if (param_pos <= last_pos)
			_params.push_back(message.substr(pos, param_pos - pos));
		if (param_pos != std::string::npos && param_pos != message.find_first_of("\r\n:"))
			param_pos++;
		pos = param_pos;
	}
	if (param_pos < last_pos && param_pos != std::string::npos && message[param_pos] == ':')
	{
		if (message.find_first_of("\n\r: ", param_pos + 1) != std::string::npos)
			_paramsopt = message.substr(param_pos + 1,  message.find_first_of("\n\r: ", param_pos + 1) - param_pos - 1);

	}
	return (0);
}

/*char	*nick(Message& message)
{
	if (message.getParams().size() == 0)
		return (ERRNONICKNAMEGIVEN);
	message._sender.setNickname(*(message.getParams().begin()));

}*/
