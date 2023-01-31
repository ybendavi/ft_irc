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
# include "User.hpp"
# include <vector>
# include "Replies.hpp"

class	Message {

	public :
						//Message(void);
						Message(User &sender, const std::string message);
						Message	&operator=(const Message &ref);
						//Message(const Message &ref);
						~Message(void);
		int				parseMessage(const std::string message);
		User&				getSender(void) const;
		std::string			getParamsopt(void) const;
		//User&				getReceiver(void) const;
		std::string			getMessage(void) const;
		std::string			getCommand(void) const;
		std::vector<std::string>	getParams(void) const;

	private :
		User				& _sender;
		//User				& _receiver;
		std::string			_receiver;
		std::string			_message;
		std::string	  		_command;
		std::vector<std::string>	_params;
		std::string			_paramsopt;

};

char	*nick(Message& message);

#endif
