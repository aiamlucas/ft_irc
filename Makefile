CXX       = c++
CXXFLAGS  = -Wall -Wextra -Werror -std=c++98
CPPFLAGS  = -Iincludes
NAME      = ircserv

ifeq ($(DEBUG),1)
CXXFLAGS += -g -O0
endif

SRC     =  src/main.cpp
SRC     += src/Utils.cpp
SRC     += src/engine/Server.cpp
SRC     += src/domain/Channel.cpp

OBJ     =  $(SRC:.cpp=.o)

TEST_NAME =  test_protocol
TEST_SRC  =  tests/test_parser.cpp
TEST_SRC  += src/protocol/Parser.cpp

TEST_OBJ  =  $(TEST_SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "$(NAME) compiled"

test-protocol: $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(TEST_OBJ) -o $(TEST_NAME)
	./$(TEST_NAME)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TEST_OBJ)

fclean: clean
	rm -f $(NAME) $(TEST_NAME)

re: fclean all

.PHONY: all clean fclean re test-protocol
