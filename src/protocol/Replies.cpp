#include "Replies.hpp"

#include "Client.hpp"

std::string numericReply(const Client& client, const std::string& numberCode,
                         const std::string& info) {
    std::string target = client.getNickname();
    if (target.empty()) target = "*";
    return std::string(":") + serverName + " " + numberCode + " " + target +
           " " + info;
}

std::string pongReply(const std::string& token) {
    return std::string(":") + serverName + " PONG " + serverName + " :" + token;
}
