#include "Handlers.hpp"

#include <cstddef>
#include <string>

#include "Client.hpp"
#include "IrcMacros.hpp"
#include "Message.hpp"
#include "Parser.hpp"
#include "Replies.hpp"

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

// lookup table
static const Command commands[] = {
    {"JOIN", handleJoin},
    {"NICK", handleNick},
    {"PING", handlePing},
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
