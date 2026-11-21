#include "Handlers.hpp"

#include <cstddef>
#include <string>

#include "Client.hpp"
#include "IrcMacros.hpp"
#include "Message.hpp"
#include "Parser.hpp"
#include "Replies.hpp"
#include "Server.hpp"

// a function pointer (calling through a lookup table)
typedef void (*Handler)(Server&, Client&, const Message&);

// a row in the lookup table (command name, handler function)
// ex: {"JOIN", handleJoin}
struct Command {
    const char* name;
    Handler handler;
};

// message should be: JOIN <#channel>
static void handleJoin(Server& server, Client& client, const Message& message) {
    if (message.params.empty()) {
        client.queue(numericReply(client, ERR_NEEDMOREPARAMS,
                                  "JOIN " ERR_NEEDMOREPARAMS_MSG));
        return;
    }
    // TODO: find or create the channel, add the client, tell the other members
    // that this client entered the channel
    std::string channelName = message.params[0];
    
    server.getState().handleJoin(client.getFd(), channelName);
    
    Channel* channel = server.getState().getChannel(channelName);
    
    std::string joinMsg = ":" + client.getNickname() + " JOIN " + channelName + "\r\n";
    
    std::vector<int> targets = channel->getBroadcastTargets(-1);
    for (size_t i = 0; i < targets.size(); i++) {
        server.queueMessage(targets[i], joinMsg);
    }
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
}

// lookup table
static const Command commands[] = {
    {"JOIN", handleJoin},
    {"NICK", handleNick},
    {"PING", handlePing},
    {"PASS", handlePass},
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
