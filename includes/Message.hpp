/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 18:26:30 by ccottin           #+#    #+#             */
/*   Updated: 2023/02/10 17:22:27 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <vector>
# include "Replies.hpp"

class	Message {

	public :

			Message(std::string);
			Message	&operator=(const Message &ref);
			Message(const std::string nickname, const std::string username, const std::string message, const std::string host);
			~Message(void);


		int							parseMessage(const std::string message);
		std::string					getParamsopt(void) const;
		std::string					getToSend(void) const;
		std::string					getMessage(void) const;
		std::string					getCommand(void) const;
		std::vector<std::string>	getParams(void) const;
		bool						getColon(void);
		void						setToSend(std::string message);
	

	private :

		std::string					_receiver;
		std::string					_message;
		std::string					_command;
		std::vector<std::string>	_params;
		std::string					_paramsopt;
		std::string					_tosend;
		bool						_colon;

};


#endif
