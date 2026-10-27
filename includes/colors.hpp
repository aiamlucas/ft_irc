#ifndef COLORS_HPP
#define COLORS_HPP

/*
To update :
#include "../colors.hpp"

Color codes : https://i.sstatic.net/9UVnC.png
1;	Bold
2;	Dim
4;	Underligned
7;	Revert (Swap Foreground/Background)
+10	Background (Render depends on terminal theme)
*/
#define RESET "\e[0m"

#define CL_BLACK "\e[30m"
#define CL_GREY "\e[90m"
#define CL_WHITE "\e[37m"
#define CL_RED "\e[31m"
#define CL_GREEN "\e[32m"
#define CL_YELLOW "\e[33m"
#define CL_BRI_YELLOW "\e[93m"
#define CL_PURPLE "\e[35m"
#define CL_BRIGHT_PURPLE "\e[95m"
#define CL_BLUE "\e[34m"
#define CL_LIGHT_BLUE "\e[36m"

#define CL_BLACK_BOLD "\e[1;30m"
#define CL_GREY_BOLD "\e[1;90m"
#define CL_WHITE_BOLD "\e[1;37m"
#define CL_RED_BOLD "\e[1;31m"
#define CL_GREEN_BOLD "\e[1;32m"
#define CL_YELLOW_BOLD "\e[1;33m"
#define CL_BRI_YELLOW_BOLD "\e[1;93m"
#define CL_PURPLE_BOLD "\e[1;35m"
#define CL_BRIGHT_PURPLE_BOLD "\e[1;95m"
#define CL_BLUE_BOLD "\e[1;34m"
#define CL_LIGHT_BLUE_BOLD "\e[1;36m"

#define CL_BLACK_DIM "\e[2;30m"
#define CL_GREY_DIM "\e[2;90m"
#define CL_WHITE_DIM "\e[2;37m"
#define CL_RED_DIM "\e[2;31m"
#define CL_GREEN_DIM "\e[2;32m"
#define CL_YELLOW_DIM "\e[2;33m"
#define CL_BRI_YELLOW_DIM "\e[2;93m"
#define CL_PURPLE_DIM "\e[2;35m"
#define CL_BRIGHT_PURPLE_DIM "\e[2;95m"
#define CL_BLUE_DIM "\e[2;34m"
#define CL_LIGHT_BLUE_DIM "\e[2;36m"

#define CL_BLACK_UNDERLIGNED "\e[4;30m"
#define CL_GREY_UNDERLIGNED "\e[4;90m"
#define CL_WHITE_UNDERLIGNED "\e[4;37m"
#define CL_RED_UNDERLIGNED "\e[4;31m"
#define CL_GREEN_UNDERLIGNED "\e[4;32m"
#define CL_YELLOW_UNDERLIGNED "\e[4;33m"
#define CL_BRI_YELLOW_UNDERLIGNED "\e[4;93m"
#define CL_PURPLE_UNDERLIGNED "\e[4;35m"
#define CL_BRIGHT_PURPLE_UNDERLIGNED "\e[4;95m"
#define CL_BLUE_UNDERLIGNED "\e[4;34m"
#define CL_LIGHT_BLUE_UNDERLIGNED "\e[4;36m"

#define CL_BLACK_REVERT "\e[7;30m"
#define CL_GREY_REVERT "\e[7;90m"
#define CL_WHITE_REVERT "\e[7;37m"
#define CL_RED_REVERT "\e[7;31m"
#define CL_GREEN_REVERT "\e[7;32m"
#define CL_YELLOW_REVERT "\e[7;33m"
#define CL_BRI_YELLOW_REVERT "\e[7;93m"
#define CL_PURPLE_REVERT "\e[7;35m"
#define CL_BRIGHT_PURPLE_REVERT "\e[7;95m"
#define CL_BLUE_REVERT "\e[7;34m"
#define CL_LIGHT_BLUE_REVERT "\e[7;36m"

#define CL_BLACK_BG "\e[40m"
#define CL_GREY_BG "\e[100m"
#define CL_WHITE_BG "\e[47m"
#define CL_RED_BG "\e[41m"
#define CL_GREEN_BG "\e[42m"
#define CL_YELLOW_BG "\e[43m"
#define CL_BRI_YELLOW_BG "\e[103m"
#define CL_PURPLE_BG "\e[45m"
#define CL_BRIGHT_PURPLE_BG "\e[105m"
#define CL_BLUE_BG "\e[44m"
#define CL_LIGHT_BLUE_BG "\e[46m"

#endif
