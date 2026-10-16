NAME = OiPoulette!

CC = c++

CFLAGS = -g -std=c++98 #-Wall -Wextra -Werror

SRCS =	src/main.cpp \
		src/Utils.cpp \

OBJS = $(SRCS:.cpp = .o)

all: $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
