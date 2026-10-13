/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbueno-m <lbueno-m@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 09:45:38 by lbueno-m          #+#    #+#             */
/*   Updated: 2026/10/12 21:41:31 by lbueno-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>

#include "Client.hpp"

class Channel {
private:
    std::string _name;
    std::string _topic;
    // Other private attributes:
    //     - operators
    //     - members
    //     - modes
    //          - i --> only invited clients can join
    //          - t --> only operators can change topic
    //          - k --> a password is required to join
    //          - o --> give (+o) or take (-o) operator power
    //          - l --> maximum number of members allowed
public:
    Channel(const std::string& name);

    // Member functions:
    // - void join()       --> for joining a channel
    // - void part()       --> for leaving the channel
    // - void kick()       --> for being kicked from a channel
    // - bool isOperator() --> is this client operator from this channel?
    // - bool isMember()   --> is this client currently in this channel?
    // - void broadcast()  --> send a broadcast (message for everyone) for the
    // people in the channel
};

#endif
