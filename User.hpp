/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 18:30:32 by ccottin           #+#    #+#             */
/*   Updated: 2023/01/29 13:24:22 by ccottin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <poll.h>
#include <string>
#include "Replies.hpp"

class	User {

	public :
		
		User(void);
		User(int socket);
		User(const User &ref);
		~User(void);
		User	&operator=(const User &ref);


		/*Accessors*/
		bool		getOp(void) const;
		bool		getInv(void) const;
		bool		getWal(void) const;
		bool		getOnline(void) const;
		std::string	getRealname(void) const;
		std::string	getIp(void) const;
		std::string	getUsername(void) const;
		std::string	getNickname(void) const;
		std::string	getPass(void) const;
		int			getSocket(void) const;

		void		setOp(bool);
		void		setInv(bool);
		void		setWal(bool);
		void		setRealname(std::string);
		void		setUsername(std::string);
		void		setNickname(std::string);
		void		setPass(std::string);

		/*parser*/

		void		parseUser(char *buffer);


	private :
		
		/*user modes*/

		bool		_isOperator;
		bool		_isInvisible;
		bool		_isWallopable;
		bool		_online;

		std::string	_username;
		std::string _ip;
		std::string	_realname;
		std::string	_nickname; // max nine char
		std::string	_pass;

		int			_socket;
};

#endif
