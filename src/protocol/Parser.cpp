#include "Parser.hpp"

#include <cctype>
#include <cstddef>
#include <string>

// an IRC line looks like:
//
// [:prefix] COMMAND [param_1 param_2 ...] [:trailing]
//
// Ex: ":ssin!ssin@192.168.0.4 PRIVMSG #42berlin :hey Oipoulette people!"
//
// prefix  -> "ssin!ssin@192.168.0.4" (prefix is optional and starts with ':')
// command -> "PRIVMSG"
// params  -> ["#42berlin", "hey Oipoulette people!"]
//
// spaces are delimiters
//
// trailing is always the last param.
// It starts with ':' as first char and it's includes the rest of line

// without ':' there is no prefix (pos stays at 0)
static void parsePrefix(const std::string& line, std::size_t& pos,
                        Message& parsed) {
    if (line.empty() || line[0] != ':') return;
    std::size_t end = line.find(' ');
    // no space found (npos): the prefix goes until end of the line
    if (end == std::string::npos) end = line.size();
    parsed.prefix = line.substr(1, end - 1);
    pos = end;
}

// reads one word starting at pos
// false -> no word left in the line
// true  -> stores the word
static bool readWord(const std::string& line, std::size_t& pos,
                     std::string& word) {
    pos = line.find_first_not_of(' ', pos);
    if (pos == std::string::npos) {
        pos = line.size();  // only spaces or nothing left
        return false;
    }
    std::size_t end = line.find(' ', pos);
    // no more spaces: this is the last word.
    if (end == std::string::npos) end = line.size();
    word = line.substr(pos, end - pos);
    pos = end;
    return true;
}

// parameters comes after the COMMAND
// it can be max 15 params (RFC 2812)
static void parseParams(const std::string& line, std::size_t& pos,
                        Message& parsed) {
    std::string word;
    const std::size_t maxParams = 15;  // RFC 2812 - limit of 15 parameters
    while (true) {
        pos = line.find_first_not_of(' ', pos);
        if (pos == std::string::npos) return;           // only spaces left
        if (parsed.params.size() >= maxParams) return;  // rest is ignored
        // ':' as first char: trailing starts
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

// TODO: message length limit => 512 bytes. Maybe truncate it in the engine!
// See the issue on github: Message length limit (512 bytes) #5
Message parseLine(const std::string& receivedLine) {
    Message parsed;
    std::size_t pos = 0;

    // storing the line without "\r\n"
    // this is a temporary solution, maybe should take out the "\r\n" direct in
    // the engine
    const std::string line =
        receivedLine.substr(0, receivedLine.find_first_of("\r\n"));

    parsePrefix(line, pos, parsed);
    if (!readWord(line, pos, parsed.command)) return parsed;
    toUpper(parsed.command);
    parseParams(line, pos, parsed);
    return parsed;
}
