/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 18:26:30 by ccottin           #+#    #+#             */
/*   Updated: 2023/02/03 17:53:32 by ccottin          ###   ########.fr       */
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
						Message(std::string);
						Message	&operator=(const Message &ref);
						//Message(const Message &ref);
						~Message(void);
		int				parseMessage(const std::string message);
		std::string			getParamsopt(void) const;
		std::string			getToSend(void) const;
		std::string			getMessage(void) const;
		std::string			getCommand(void) const;
		std::vector<std::string>	getParams(void) const;
		void				setToSend(std::string message);
		void				setSender(std::string sender);
		std::string			setPrefix(std::string server_name);
	private :
		//User				& _receiver;
		std::string			_receiver;
		std::string			_sender;
		std::string			_message;
		std::string	  		_command;
		std::vector<std::string>	_params;
		std::string			_paramsopt;
		std::string			_tosend;

};


#endif
