/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccottin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 18:30:32 by ccottin           #+#    #+#             */
/*   Updated: 2023/02/01 18:22:19 by ybendavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <unistd.h>
#include <poll.h>
#include <string>
#include <list>
#include "Message.hpp"
#include "Replies.hpp"

class	User {

	public :
		
		User(void);
		User(struct pollfd socket);
		User(const User &ref);
		~User(void);
		User	&operator=(const User &ref);


		/*Accessors*/
		bool			getOp(void) const;
		bool			getInv(void) const;
		bool			getWal(void) const;
		bool			getOnline(void) const;
		std::string		getRealname(void) const;
		std::string		getIp(void) const;
		std::string		getUsername(void) const;
		std::string		getNickname(void) const;
		std::string		getPass(void) const;
		struct pollfd	&getSocket(void);
		struct pollfd	getSocket(void) const;

		void			setOp(bool);
		void			setInv(bool);
		void			setWal(bool);
		void			setRealname(std::string);
		void			setUsername(std::string);
		void			setNickname(std::string);
		void			setPass(std::string);
		void			execute(void);
		
		/*pushback le dernier lu & pop front qund on affiche*/
		std::list< Message >		receivedmsg; //will eventually become a class so...
		std::list< Message >		tosendmsg; //will eventually become a class so...
		/*simple = on push back ce que l on doit envoyer et pop front ce au on envoye*/

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

		std::string	_checkParam(void);


	public : 
		/*le temps de faire un truc propre il sera PUBLIK*/
		struct pollfd	_socket;
};

#endif
