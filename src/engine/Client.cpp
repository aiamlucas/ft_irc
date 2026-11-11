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

// add a finish line to the output buffer of the client
// the protocol layer passes a line for this client (server->client)
// it can be a numeric reply, a pong reply, a message from a user, etc
//
//  Porotocol          Client                Engine
// queue(line) ----> _outBuffer -----------> send()->socket
//                                 when socket is ready the message is sent

// "\r\n" is added to every line
// ex: _outBuffer:
//                ":ssin!ssin@127.0.0.6 NICK :sara\r\n
//                :sun!sun@127.0.0.31 JOIN #42berlin\r\n"
void Client::queue(const std::string& line) {
    const std::size_t lineLimit = 510;
    _outBuffer += line.substr(0, lineLimit);
    _outBuffer += "\r\n";
}

bool Client::hasOutput() const {
    if (_outBuffer.empty()) return false;
    return true;
}

const std::string& Client::getOutBuffer() const { return _outBuffer; }

void Client::eraseOutBuffer(std::size_t n_bytes) {
    _outBuffer.erase(0, n_bytes);
}
