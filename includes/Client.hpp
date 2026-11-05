#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
private:
    int _fd;
    std::string _inBuffer;
    std::string _outBuffer;
    std::string _nickname;
    std::string _username;
    std::string _hostname;  // client IP -> used in prefix

    bool _passwordOk;
    bool _welcomeOk;  // welcome messages sent: 001, 002, 003, 004, 422(MOTD)

public:
    Client(int fd);
    ~Client();
    int getFd() const;

    void setNickname(const std::string& nick);
    void setUsername(const std::string& username);
    void setHostname(const std::string& hostname);
    void setPasswordOk();
    void setWelcomeOk();

    std::string& getInBuffer();
    std::string getNickname() const;
    std::string getUsername() const;
    std::string getHostname() const;
    bool isPasswordOk() const;
    bool isWelcomeOk() const;
    bool isRegistered() const;   // password + username + nickname
    std::string prefix() const;  // "nickname!username@hostname"
};

#endif
