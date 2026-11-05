#include "Client.hpp"

Client::Client(int fd) : _fd(fd), _passwordOk(false), _welcomeOk(false) {}

Client::~Client() {}

int Client::getFd() const { return _fd; }

void Client::setNickname(const std::string& nickname) { _nickname = nickname; }
void Client::setUsername(const std::string& username) { _username = username; }
void Client::setHostname(const std::string& hostname) { _hostname = hostname; }
void Client::setPasswordOk() { _passwordOk = true; }
void Client::setWelcomeOk() { _welcomeOk = true; }

std::string& Client::getInBuffer() { return _inBuffer; }
std::string Client::getNickname() const { return _nickname; }
std::string Client::getUsername() const { return _username; }
std::string Client::getHostname() const { return _hostname; }

bool Client::isPasswordOk() const { return _passwordOk; }
bool Client::isWelcomeOk() const { return _welcomeOk; }
// a client is registered when it has entered the right password
// and has a nickname and username
bool Client::isRegistered() const {
    return _passwordOk && !_nickname.empty() && !_username.empty();
}

std::string Client::prefix() const {
    return _nickname + "!" + _username + "@" + _hostname;
}
