/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbueno-m <lbueno-m@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 09:49:39 by lbueno-m          #+#    #+#             */
/*   Updated: 2026/10/13 09:49:51 by lbueno-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

#include <climits>

static bool checkOverFlow(int num, int digit, int sign) {
    if (sign == 1) {
        if (num > (INT_MAX - digit) / 10) return true;
    } else {
        unsigned int limit = static_cast<unsigned int>(INT_MAX) + 1;
        if (static_cast<unsigned int>(num) > (limit - digit) / 10) return true;
    }
    return false;
}

static const char* skipWhitespace(const char* str) {
    while (*str == ' ' || (*str >= '\t' && *str <= '\r')) str++;
    return str;
}

static const char* parseSign(const char* str, int& sign) {
    sign = 1;
    if (*str == '+' || *str == '-') {
        if (*str == '-') sign = -1;
        str++;
    }
    return str;
}

bool safeAtoi(const char* str, int& result) {
    int sign;
    int num = 0;
    int digit;

    str = skipWhitespace(str);
    str = parseSign(str, sign);
    if (!(*str >= '0' && *str <= '9')) return false;
    while (*str >= '0' && *str <= '9') {
        digit = *str - '0';
        if (checkOverFlow(num, digit, sign)) return false;
        num = num * 10 + digit;
        str++;
    }
    str = skipWhitespace(str);
    if (*str != '\0') return false;
    result = num * sign;
    return true;
}
