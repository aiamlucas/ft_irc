#include "ServerState.hpp"

ServerState::ServerState() {}
ServerState::~ServerState() {}

void ServerState::handleJoin(int fd, const std::string& channelName) {
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);

    if (it == _channels.end()) {
        Channel* newChannel = new Channel(channelName);

        _channels[channelName] = newChannel;
        it = _channels.find(channelName);
    }
    it->second->join(fd);
}

void ServerState::handlePart(int fd, const std::string& channelName) {
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);

    if (it == _channels.end()) {
        return;
    }

    Channel* channel = it->second;
    channel->part(fd);

    if (channel->isEmpty()) {
        delete channel;
        _channels.erase(it);
    }
    // todo: send message to all other mebers, broadcast
}

Channel* ServerState::getChannel(const std::string& channelName) {
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (it != _channels.end()) {
        return it->second;
    }
    return NULL;
}
