#ifndef UTILS_HPP
#define UTILS_HPP

#include <climits>  // INT_MAX
#include <iostream>
#include <string>

#define INVALID_ARGS -1

int argsCheck(char* argv[]);
bool safeAtoi(const char* str, int& result);

#endif
