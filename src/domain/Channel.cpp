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

// set topic
void Channel::setTopic(const std::string& topic) {
	_topic = topic;
}

// Membership functions
bool Channel::isMember(int fd) const {
	if (_members.find(fd) != _members.end()) {
		return true;
	}
	return false;
}
void Channel::join(int fd) {
	if (isMember(fd)) {
		return;
	}
	_members.insert(fd);
	if (_members.size() == 1) {
		_operators.insert(fd);
	}
}
void Channel::part(int fd) {
	if (isOperator(fd)) {
		removeOperator(fd);
	}
	_members.erase(fd);
}
void Channel::kick(int fd) {
	if (isOperator(fd)) {
		removeOperator(fd);
	}
	_members.erase(fd);
}

// Operators
bool Channel::isOperator(int fd) const {
	if (_operators.find(fd) != _operators.end()) {
		return true;
	}
	return false;
}
void Channel::addOperator(int fd) {
	if (!isMember(fd) || isOperator(fd)) {
		return;
	}
	_operators.insert(fd);
}
void Channel::removeOperator(int fd) {
	_operators.erase(fd);
}

// Modes
bool Channel::isInviteOnly() const {
	return _inviteOnly;
}
void Channel::setInviteOnly(bool value) {
	_inviteOnly = value;
}

bool Channel::isTopicRestricted() const {
	return _topicRestricted;
}

void Channel::setTopicRestricted(bool value) {
	_topicRestricted = value;
}

const std::string& Channel::getKey() const {
	return _key;
}

void Channel::setKey(const std::string& key) {
	_key = key;
}

size_t Channel::getLimit() const {
	return _limit;
}

void Channel::setLimit(size_t limit) {
	_limit = limit;
}

// additional helper functions
bool Channel::isEmpty() const {
	return _members.empty();
}

std::ostream& operator<<(std::ostream& os, const Channel& other) {
    os << "Channel name: " << other.getName() << '\n'
       << "Channel topic: " << other.getTopic();
    return (os);
}