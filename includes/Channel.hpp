/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 20:02:03 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/03 20:48:02 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <map>

# define OPERATOR   1
# define VOICE      2
# define BAN        4
# define INVITE     8
# define MODERATED  16
# define TOPIC      32

class Channel
{

    public:

        Channel(std::string name);  //names for channel must follow some rules (begin with a # character, must not contain ' ' or ',')
        ~Channel();

        void    addUser(const std::string& user, unsigned char modes); //calling function MUST check if user really exist before calling that method
    
    private:

        std::string                             _channel;   //channel name
        std::map<std::string, unsigned char>    _chanUsers; //nickname of the user and bitmask on the specific channel
};

#endif