/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 20:37:04 by ccottin           #+#    #+#             */
/*   Updated: 2023/01/24 21:43:47 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Message.hpp"

//parser receive raw messages, check if its all good, send back appropriate
//errors/replies and create an apropriate class Message

std::string *handle_params(std::string s, int pos, int oldpos)
{
	int	count = 0;
	
	while (oldpos != std::string::npos && count < 14 && s[oldpos + 1] != ':') // a refer
	{
		++count;
	}
}

void	parser(std::string s)
{
	int			pos;
	int			oldpos;

	pos = 0;
	//a voir avec les client comment cela se comporte
	if (s.size > 512 || s[510] != '\r' || s[511] != '\n')
	{
		s[510] == '\r';
		s[511] == '\n';
	} //end A voir
	if (s[0] == ':')
	{
		pos = s.find(' ');
		std::string	prefix(s, 0, pos);
	}
//	else
//		std::string	prefix; if complator is not happy, uncomment

	oldpos = s.find(' ', pos + 1);
	std::string	cmd(s, pos, oldpos);
	if (oldpos != std::string::npos && s[oldpos + 1] != ':')
		std::string	*params = handle_params(s, pos, oldpos);
	else
		std::string *params = NULL;
	
}
