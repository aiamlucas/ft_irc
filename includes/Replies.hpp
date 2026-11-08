#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>

class Client;

std::string numericReply(const Client& client, const std::string& numberCode,
                         const std::string& info);
std::string pongReply(const std::string& token);

#endif
