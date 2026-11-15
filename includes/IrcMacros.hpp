#ifndef IRC_MACROS_HPP
#define IRC_MACROS_HPP

// server
#define SERVER_NAME "oipoulette"
#define SERVER_VERSION "0.9"
#define CHANNEL_MODES "itkol"  // sent in 004
#define USER_MODES "o"         // sent in 004

// limits
#define MAX_LINE_LENGTH 510  // without "\r\n"
#define MAX_PARAMS 15
#define MAX_NICK_LENGTH 9

// Numeric Replies
// registration (welcome block)
#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATED "003"
#define RPL_MYINFO "004"

// numeric errors
#define ERR_UNKNOWNCOMMAND "421"    // <command> :Unknown command
#define ERR_NOMOTD "422"            // :MOTD File is missing
#define ERR_NONICKNAMEGIVEN "431"   // :No nickname given
#define ERR_ERRONEUSNICKNAME "432"  // <nick> :Erroneous nickname
#define ERR_NICKNAMEINUSE "433"     // <nick> :Nickname is already in use
#define ERR_NOTREGISTERED "451"     // :You have not registered
#define ERR_NEEDMOREPARAMS "461"    // <command> :Not enough parameters
#define ERR_ALREADYREGISTRED "462"  // :You may not reregister
#define ERR_PASSWDMISMATCH "464"    // :Password incorrect

// messages: replies
#define RPL_WELCOME_MSG ":Welcome to the IRC Network "
#define RPL_YOURHOST_MSG \
    ":Your host is " SERVER_NAME ", running version " SERVER_VERSION
#define RPL_CREATED_MSG ":This server was created " __DATE__
#define RPL_MYINFO_MSG \
    SERVER_NAME " " SERVER_VERSION " " USER_MODES " " CHANNEL_MODES

// messages: errors
#define ERR_UNKNOWNCOMMAND_MSG ":Unknown command"
#define ERR_NOMOTD_MSG ":MOTD File is missing"
#define ERR_NONICKNAMEGIVEN_MSG ":No nickname given"
#define ERR_ERRONEUSNICKNAME_MSG ":Erroneous nickname"
#define ERR_NICKNAMEINUSE_MSG ":Nickname is already in use"
#define ERR_NOTREGISTERED_MSG ":You have not registered"
#define ERR_NEEDMOREPARAMS_MSG ":Not enough parameters"
#define ERR_ALREADYREGISTRED_MSG ":You may not reregister"
#define ERR_PASSWDMISMATCH_MSG ":Password incorrect"

#endif
