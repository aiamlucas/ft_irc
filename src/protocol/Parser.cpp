#include "Parser.hpp"

#include <cstddef>
#include <string>

static void parsePrefix(const std::string& line, std::size_t& pos,
                        Message& parsed) {
    if (line.empty() || line[0] != ':') return;
    std::size_t end = line.find(' ');
    // no space found (npos): the prefix goes until end of the line
    if (end == std::string::npos) end = line.size();
    parsed.prefix = line.substr(1, end - 1);
    pos = end;
}

static bool readWord(const std::string& line, std::size_t& pos,
                     std::string& word) {
    pos = line.find_first_not_of(' ', pos);
    if (pos == std::string::npos) {
        pos = line.size();
        return false;
    }
    std::size_t end = line.find(' ', pos);
    if (end == std::string::npos) end = line.size();
    word = line.substr(pos, end - pos);
    pos = end;
    return true;
}

static void parseParams(const std::string& line, std::size_t& pos,
                        Message& parsed) {
    std::string word;
    while (true) {
        pos = line.find_first_not_of(' ', pos);
        if (pos == std::string::npos) return;
        if (line[pos] == ':') {
            parsed.params.push_back(line.substr(pos + 1));
            return;
        }
        readWord(line, pos, word);
        parsed.params.push_back(word);
    }
}

static void toUpper(std::string& str) {
    for (std::size_t i = 0; i < str.size(); i++)
        str[i] =
            static_cast<char>(std::toupper(static_cast<unsigned char>(str[i])));
}

Message parseLine(const std::string& receivedLine) {
    Message parsed;
    std::size_t pos = 0;
    // storing the line without "\r\n"
    const std::string line =
        receivedLine.substr(0, receivedLine.find_first_of("\r\n"));

    parsePrefix(line, pos, parsed);
    if (!readWord(line, pos, parsed.command)) return parsed;
    toUpper(parsed.command);
    parseParams(line, pos, parsed);
    return parsed;
}
