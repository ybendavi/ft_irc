/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdapurif <cdapurif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 21:36:55 by cdapurif          #+#    #+#             */
/*   Updated: 2023/02/13 14:09:50 by cdapurif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal.hpp"

extern volatile sig_atomic_t loop;

void    sighandler(int signum)
{
    (void)signum;

    std::cout << std::endl;

    loop = 0;
}