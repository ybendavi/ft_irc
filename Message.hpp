/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 18:26:30 by ccottin           #+#    #+#             */
/*   Updated: 2023/01/25 15:57:55 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include "User.hpp"

class	Message {

	public :
		Message(void);
		Message(const User &Sender, const User &Receiver,
				const std::string content, const std::string command);
		Message	&operator=(const Message &ref);
		Message(const Message &ref);
		~Message(void);

	private :
		User		& _sender;
		User		& _receiver;
		std::string	  _content;
		std::string	  _command;
		std::string	  _prefix;
		std::string	* _params;

};

#endif
