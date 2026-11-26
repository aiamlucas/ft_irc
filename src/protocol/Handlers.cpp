#include "Handlers.hpp"

#include <cstddef>
#include <string>

#include "Client.hpp"
#include "IrcMacros.hpp"
#include "Message.hpp"
#include "Parser.hpp"
#include "Replies.hpp"
#include "Server.hpp"

// sends the welcome messages (001-004)
// when the client finished the registration (password + nickname + username)
static void welcomeIfRegistered(Client& client) {
    if (!client.isRegistered() || client.isWelcomeOk()) return;
    client.setWelcomeOk();
    client.queue(
        numericReply(client, RPL_WELCOME, RPL_WELCOME_MSG + client.prefix()));
    client.queue(numericReply(client, RPL_YOURHOST, RPL_YOURHOST_MSG));
    client.queue(numericReply(client, RPL_CREATED, RPL_CREATED_MSG));
    client.queue(numericReply(client, RPL_MYINFO, RPL_MYINFO_MSG));
    client.queue(numericReply(client, ERR_NOMOTD, ERR_NOMOTD_MSG));
}

// a function pointer (calling through a lookup table)
typedef void (*Handler)(Server&, Client&, const Message&);

// a row in the lookup table (command name, handler function)
// ex: {"JOIN", handleJoin}
struct Command {
    const char* name;
    Handler handler;
};

// message should be: JOIN <#channel>
static void handleJoin(Server&, Client& client, const Message& message) {
    if (message.params.empty()) {
        client.queue(numericReply(client, ERR_NEEDMOREPARAMS,
                                  "JOIN " ERR_NEEDMOREPARAMS_MSG));
        return;
    }
    // TODO: find or create the channel, add the client, tell the other members
    // that this client entered the channel
}

// message should be: NICK <nickname>
static void handleNick(Server&, Client& client, const Message& message) {
    if (message.params.empty()) {
        client.queue(
            numericReply(client, ERR_NONICKNAMEGIVEN, ERR_NONICKNAMEGIVEN_MSG));
        return;
    }
    client.setNickname(message.params[0]);
    welcomeIfRegistered(client);
}

// message should be: PING <token>
static void handlePing(Server&, Client& client, const Message& message) {
    if (message.params.empty()) {
        client.queue(numericReply(client, ERR_NEEDMOREPARAMS,
                                  "PING " ERR_NEEDMOREPARAMS_MSG));
        return;
    }
    client.queue(pongReply(message.params[0]));
}

static void handlePass(Server& server, Client& client, const Message& message) {
    if (message.params.empty()) {
        client.queue(numericReply(client, ERR_NEEDMOREPARAMS,
                                  "PASS " ERR_NEEDMOREPARAMS_MSG));
        return;
    }
    if (client.isRegistered()) {
        client.queue(numericReply(client, ERR_ALREADYREGISTRED,
                                  ERR_ALREADYREGISTRED_MSG));
        return;
    }
    if (!server.isPasswordAccepted(message.params[0])) {
        client.queue(
            numericReply(client, ERR_PASSWDMISMATCH, ERR_PASSWDMISMATCH_MSG));
        // TODO: close the connection after ERR_PASSWDMISMATCH (issue #10)
        return;
    }
    client.setPasswordOk();
    welcomeIfRegistered(client);
}

// message should be: USER <username> <mode> <unused> :<realname>
// EX: USER ssin 0 * :Sara Sin
static void handleUser(Server&, Client& client, const Message& message) {
    if (message.params.size() < 4) {
        client.queue(numericReply(client, ERR_NEEDMOREPARAMS,
                                  "USER " ERR_NEEDMOREPARAMS_MSG));
        return;
    }
    if (client.isRegistered()) {
        client.queue(numericReply(client, ERR_ALREADYREGISTRED,
                                  ERR_ALREADYREGISTRED_MSG));
        return;
    }
    client.setUsername(message.params[0]);
    welcomeIfRegistered(client);
}

// lookup table
static const Command commands[] = {
    {"JOIN", handleJoin}, {"NICK", handleNick}, {"PING", handlePing},
    {"PASS", handlePass}, {"USER", handleUser},
};

// finds the right command in the lookup table and calls its handler
void handleLine(Server& server, Client& client, const std::string& line) {
    Message message = parseLine(line);
    if (message.command.empty()) return;

    // search the handler in the lookup table
    const std::size_t numberOfRows = sizeof(commands) / sizeof(commands[0]);
    for (std::size_t i = 0; i < numberOfRows; i++) {
        if (message.command == commands[i].name) {
            commands[i].handler(server, client, message);
            return;
        }
    }
    // command is not in the lookup table
    client.queue(numericReply(client, ERR_UNKNOWNCOMMAND,
                              message.command + " " ERR_UNKNOWNCOMMAND_MSG));
}
