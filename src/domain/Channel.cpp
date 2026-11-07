#include "Channel.hpp"

// Parametrized constructor
Channel::Channel(const std::string& name)
    : _name(name),
      _topic(""),
      _inviteOnly(false),
      _topicRestricted(false),
      _limit(0) {}

// destructor
Channel::~Channel() {}

// Getters
const std::string& Channel::getName() const { return _name; }

const std::string& Channel::getTopic() const { return _topic; }

const std::set<int>& Channel::getMembers() const { return _members; }

std::ostream& operator<<(std::ostream& os, const Channel& other) {
    os << "Channel name: " << other.getName() << '\n'
       << "Channel topic: " << other.getTopic();
    return (os);
}