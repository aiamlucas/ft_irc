#include "Replies.hpp"

#include "Client.hpp"
#include "IrcMacros.hpp"

std::string numericReply(const Client& client, const std::string& numberCode,
                         const std::string& info) {
    std::string nick = client.getNickname();
    // "*" if the client has no nickname yet
    if (nick.empty()) nick = "*";
    return ":" SERVER_NAME " " + numberCode + " " + nick + " " + info;
}

std::string pongReply(const std::string& token) {
    return ":" SERVER_NAME " PONG " SERVER_NAME " :" + token;
}
