#include "Client.hpp"

Client::Client(int fd) : _fd(fd), _passwordOk(false), _welcomeOk(false) {}

Client::~Client() {}

int Client::getFd() const { return _fd; }

void Client::setNickname(const std::string& nickname) { _nickName = nickname; }
void Client::setUserName(const std::string& username) { _userName = username; }
void Client::setHostName(const std::string& hostname) { _hostName = hostname; }
void Client::setPasswordOk() { _passwordOk = true; }
void Client::setWelcomeOk() { _welcomeOk = true; }

std::string& Client::getInBuffer() { return _inBuffer; }
std::string Client::getNickname() const { return _nickName; }
std::string Client::getUserName() const { return _userName; }
std::string Client::getHostName() const { return _hostName; }

bool Client::isPasswordOk() const { return _passwordOk; }
bool Client::isWelcomeOk() const { return _welcomeOk; }
// a client is registered when it has entered the right password
// and has a nickname and username
bool Client::isRegistered() const {
    return _passwordOk && !_nickName.empty() && !_userName.empty();
}

std::string Client::prefix() const {
    return _nickName + "!" + _userName + "@" + _hostName;
}
