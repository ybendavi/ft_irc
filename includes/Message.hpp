/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 18:26:30 by ccottin           #+#    #+#             */
/*   Updated: 2023/01/29 23:10:20 by ybendavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <vector>
# include "Replies.hpp"

class	Message {

	public :
						//Message(void);
						Message(const char *);
						Message	&operator=(const Message &ref);
						//Message(const Message &ref);
						~Message(void);
		int				parseMessage(const std::string message);
		std::string			getParamsopt(void) const;
		std::string			getToSend(void) const;
		std::string			getMessage(void) const;
		std::string			getCommand(void) const;
		std::vector<std::string>	getParams(void) const;

	private :
		//User				& _receiver;
		std::string			_receiver;
		std::string			_message;
		std::string	  		_command;
		std::vector<std::string>	_params;
		std::string			_paramsopt;
		std::string			_tosend;

};

char	*nick(Message& message);

#endif
