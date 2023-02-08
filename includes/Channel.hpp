/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 20:02:03 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/08 15:07:15 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <map>

// USER MODES
# define OPERATOR   1
# define VOICE      2
# define BAN        4
# define INVITE     8
# define MODERATED  16
# define TOPIC      32

// CHANNEL MODES
# define CHANBAN    1
# define EXCEPTION  2
# define LIMIT      4
# define INVONLY    8
# define INVEXCEPT  16
# define KEY        32
# define MODCHAN    64
# define SECRET     128
# define PROTECTOP  256
# define EXTERNMESS 512

class Channel
{

    public:

        Channel(std::string name, unsigned short modes = PROTECTOP | EXTERNMESS);
        ~Channel();

        void    addUser(const std::string& user, unsigned char modes = VOICE | INVITE);
    
    private:

        std::string                             _channel;   //channel name
        unsigned short                          _chanMode;  //channel modes
        std::map<std::string, unsigned char>    _chanUsers; //nickname of the user and bitmask on the specific channel
};

#endif