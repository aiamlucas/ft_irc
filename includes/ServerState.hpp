#ifndef SERVER_STATE
#define SERVER_STATE

#include <map>

#include "Channel.hpp"

class ServerState {
private:
    std::map<std::string, Channel*> _channels;

    ServerState(const ServerState& other);
    ServerState& operator=(const ServerState& other);

public:
    ServerState();
    ~ServerState();

    // main domain orders
    void handleJoin(int fd, const std::string& channelName);
    void handlePart(int fd, const std::string& channelName);
    void handlePrivmsg(int fd, const std::string& target,
                       const std::string& message);
    void handleKick(int fd, const std::string& channelName,
                    const std::string& targetNick);
    void handleMode(int fd, const std::string& channelNmae,
                    const std::string& modes);

    // get channel
    Channel* getChannel(const std::string& channelName);
};

#endif