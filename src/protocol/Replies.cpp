#include "Replies.hpp"

#include "Client.hpp"

std::string numericReply(const Client& client, const std::string& numberCode,
                         const std::string& info) {
    std::string servername = "oipoulette";
    std::string nick = client.getNickname();
    // "*" if the client has no nickname
    if (nick.empty()) nick = "*";
    return ":" + servername + " " + numberCode + " " + nick + " " + info;
}

std::string pongReply(const std::string& token) {
    std::string servername = "oipoulette";
    return ":" + servername + " PONG " + servername + " :" + token;
}
