#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <set>
#include <string>

#include "Client.hpp"

class Channel {
private:
    std::string _name;
    std::string _topic;

    std::set<int> _members;    // fd of members
    std::set<int> _operators;  // fd of operators

    bool _inviteOnly;       // +i
    bool _topicRestricted;  // +t
    std::string _key;       // +k
    size_t _limit;          // +l

    // OCF
    Channel();                                 // default constructor
    Channel(const Channel& other);             // copy constructor
    Channel& operator=(const Channel& other);  // copy assignment operator
public:
    // OCF
    Channel(const std::string& name);
    ~Channel();

    // Getters
    const std::string& getName() const;
    const std::string& getTopic() const;
    const std::set<int>& getMembers() const;

    // Operators
    void addOperator(int fd);     // +o
    void removeOperator(int fd);  // -o
    void isOperator(int fd) const;  // --> is this client operator from this channel?

    // Membership
    void join(int fd);  // --> for joining a channel
    void part(int fd);  // --> for leaving the channel
    void kick(int fd);  // --> for being kicked from a channel
    bool isMember(int fd) const;  // --> is this client currently in this channel?

    // Modes
    bool isInviteOnly() const;
    void setInviteOnly();
    // - void broadcast()  --> send a broadcast (message for everyone) for the
    // people in the channel
};

std::ostream& operator<<(std::ostream& os, const Channel& other);

#endif
