#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "Message.hpp"
#include "Parser.hpp"

static int g_total = 0;
static int g_failed = 0;

// {"#sk8", "hey you!"} -> "<#sk8><hey you>"
static std::string show(const std::vector<std::string>& params) {
    std::string result;
    for (int i = 0; i < params.size(); i++) result += "<" + params[i] + ">";
    return result;
}

static void check(const std::string& line, const std::string& prefix,
                  const std::string& command, const std::string& params) {
    Message m = parseLine(line);
    g_total++;
    if (m.prefix == prefix && m.command == command && show(m.params) == params)
        return;
    g_failed++;
    std::cerr << "FAIL \"" << line << "\"\n"
              << " expected: prefix=[" << prefix << "] command=[" << command
              << "] params=" << params << "\n"
              << " got: prefix=[" << m.prefix << "] command=[" << m.command
              << "] params=" << show(m.params) << std::endl;
}

int main() {
    // basic commands
    check("PING", "", "PING", "");
}
