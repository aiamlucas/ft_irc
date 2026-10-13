/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbueno-m <lbueno-m@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 15:36:10 by lbueno-m          #+#    #+#             */
/*   Updated: 2026/10/12 15:43:51 by lbueno-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
private:
    // Engine:
    int _fd;
    std::string _inBuffer;
    // Engine + Protocol + Domain:
    std::string _outBuffer;
    // Protocol + Domain
    std::string _nickName;

    // what else?

public:
    // Engine:
    Client(int fd);
    int getFd() const;
    // Protocol
    void setNickname(const std::string& nick);
    // Protocol + Domain
    std::string getNickname() const;

    // we have to thing about the functions for each layer of the project
    // here is some info (and examples) about what each one does

    // Engine --> Recieves bytes and find where a line ends (\r\n)
    //            once having a line, it fowards it to the Protocol as a string
    //
    // Example (bytes):  4A 4F 49 4E 20 23 73 6B 38 0D 0A
    //                   J  O  I  N     #  s  k  8  \r \n
    //
    //                 Example from functions:
    //                     -> recieveData(client);
    //

    // Protocol --> Takes the string (forwarded by the Engine) and make it
    //              into a command. Ex: "command = 'JOIN', params = ['#sk8'];"
    //              Decides what to do with it:
    //              Simple commands: (handle directly): Ex: NICK, PING, etc...
    //              Channel commands: (handled by Domain) Ex: JOIN, KICK, etc...
    //
    //                  Example from functions:
    //                      -> parseLine(line);
    //                      -> dispatch(message, client);
    //                      -> handleNick(client& str);
    //
    // Domain:  --> Get called for channel commands.
    //              Changes it's state (add/remove a member, change a mode,
    //              etc),
    //                  Example from functions:
    //                      -> channel.join(client);
    //                      -> channel.broadcast(line, except);
};

#endif
