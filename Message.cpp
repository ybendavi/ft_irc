/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 18:28:25 by ccottin           #+#    #+#             */
/*   Updated: 2023/01/25 16:18:24 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message(void)
{ }

Message::Message(const Message &ref)
{ 
	*this = ref;
}

Message::~Message(void)
{ } 

Message::Message(const User &Sender, const User &Receiver,
			const std::string content, const std::string command)
	: _sender(Sender), _receiver(Receiver), _content(content),
			_command(command)
{ }

Message	&Message::operator=(const Message &ref)
{
	if (this != &ref)
	{
		this->_sender = ref._sender;
		this->_receiver = ref._receiver;
		this->_content = ref._content;
		this->_command = ref.command;
	}
	return (*this);
}


