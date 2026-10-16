#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <climits>		// INT_MAX

using namespace std;

int argsCheck(char *argv[]);
bool safeAtoi(const char* str, int& result);

#endif
