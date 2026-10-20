#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "../includes/Message.hpp"
#include "../includes/Parser.hpp"

static int g_total = 0;
static int g_failed = 0;

// {"#42berlin", "oie pessoal!"} -> "<#42berlin><oie pessoal!>"
static std::string paramsToString(const std::vector<std::string>& params) {
    std::string result;
    for (std::size_t i = 0; i < params.size(); i++)
        result += "<" + params[i] + ">";
    return result;
}

static void expectParse(const std::string& line, const std::string& prefix,
                        const std::string& command, const std::string& params) {
    Message parsedLine = parseLine(line);
    g_total++;
    if (parsedLine.prefix == prefix && parsedLine.command == command &&
        paramsToString(parsedLine.params) == params)
        return;
    g_failed++;
    std::cerr << "FAIL \"" << line << "\"\n"
              << " expected: prefix=[" << prefix << "] command=[" << command
              << "] params=[" << params << "]" << "\n"
              << " got: prefix=[" << parsedLine.prefix << "] command=["
              << parsedLine.command << "] params=["
              << paramsToString(parsedLine.params) << "]" << std::endl;
}

int main() {
    // testing the line and the expected results

    // expectParse(line, prefix, command, params)

    // line: the raw text, that comes from the client
    //       [:prefix] COMMAND [param1, param2, ...] [:trailing]

    // Test 1
    // "PING"
    // prefix: ""
    // command: PING
    // params:  ""
    expectParse("PING", "", "PING", "");

    // Test 2
    // "NICK ssin"
    // prefix: ""
    // command: NICK
    // params: ssin
    expectParse("NICK ssin", "", "NICK", "<ssin>");

    // Test 3
    // "JOIN #42berlin"
    // prefix: ""
    // command: JOIN
    // params: #42berlin
    expectParse("JOIN #42berlin", "", "JOIN", "<#42berlin>");

    // Test 4
    // ":ssin!abcd@192.168.1.105 PRIVMSG #42berlin :oieee pessoal!"
    // prefix: ssin!abcd@192.168.1.105
    // command: PRIVMSG
    // params: #42berlin
    //         oieee pessoal!
    expectParse(":ssin!abcd@192.168.1.105 PRIVMSG #42berlin :oieee pessoal!",
                "ssin!abcd@192.168.1.105", "PRIVMSG",
                "<#42berlin><oieee pessoal!>");

    std::cout << (g_total - g_failed) << "/" << g_total << " passed"
              << std::endl;

    if (g_failed) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
