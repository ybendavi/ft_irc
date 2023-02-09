/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Updated: 2023/02/02 00:06:50 by ybendavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include <string>
#include <list>
#include "Message.hpp"
#include "Replies.hpp"

class	User {

	public :
		
		User(void);
		User(struct pollfd * socket, struct sockaddr * addr);
		User(const User &ref);
		~User(void);
		User	&operator=(const User &ref);


		/*Accessors*/
		std::string		getRealname(void) const;
		std::string		getIp(void) const;
		std::string		getUsername(void) const;
		std::string		getNickname(void) const;
		struct pollfd	*getSocket(void);
		struct pollfd	*getSocket(void) const ;
		struct sockaddr	*getAddr(void);
		struct sockaddr	*getAddr(void) const;
		char			getMode(void) const;
		bool			getPass(void) const;

		void			setIp(std::string);
		void			setMode(char m);
		void			setRealname(std::string);
		void			setUsername(std::string);
		void			setNickname(std::string);
		void			setPass(bool);
		void			setEvent(short);	
		
		std::list< Message >		receivedmsg;
		std::list< Message >		tosendmsg; 

	private :
	
		std::string	_username;
		std::string _ip;
		std::string	_realname;
		std::string	_nickname; // max nine char

		char		_mode;
		bool		_pass;

	public : 
		/*le temps de faire un truc propre il sera PUBLIK*/
		struct pollfd	*_socket;
		struct sockaddr	*_addr;
};

#endif
