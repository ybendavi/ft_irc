/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 20:02:03 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/09 11:56:13 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <map>
# include <vector>

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
        void    removeUserFromChannel(const std::string& nickname);
        bool    isUserOnChannel(const std::string& nickname);

        unsigned short  size(void);
    
    private:

        int                                     _checkUserPerms(const std::string& nickname);

        std::string                             _channel;   //channel name
        unsigned short                          _chanMode;  //channel modes
        std::map<std::string, unsigned char>    _chanUsers; //nickname of the user and bitmask on the specific channel
        std::vector<std::string>                _banList;   //list of ban people on the server (should be empty/clear if channel doesn't have ban mask)
        int                                     _chanLimit; //limit of people in the channel (should be set to -1 when limit mask isn't set)
        //std::string                             _topic[2];  //topic[0] is the chanel topic and _topic[1] is the user who set the topic
};

#endif