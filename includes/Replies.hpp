#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>

class Client;  // forward declaration

// the server answer to a request with a 3-digit number code
// :<server> <numberCode> <nickname or *> <info>
// Ex:
// ":oipoulette 461 ssin PING :Not enough parameters"
std::string numericReply(const Client& client, const std::string& numberCode,
                         const std::string& info);
// server answer to PING, no number code:
// :<server> PONG <server> :<token>
std::string pongReply(const std::string& token);

// TODO: create userReply() -> for actions of a user that the server
// forwards to others (NICK, JOIN, PRIVMSG, etc)
// client -> server -> client
// the line starts with the prefix of the user
// :<nickname!username@hostname> <command> <params>
// Ex:
// :ssin!ssin@127.0.0.6 JOIN #42berlin

#endif
