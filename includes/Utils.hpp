#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <climits>		// INT_MAX

#define INVALID_ARGS -1

int argsCheck(char *argv[]);
bool safeAtoi(const char* str, int& result);

#endif
