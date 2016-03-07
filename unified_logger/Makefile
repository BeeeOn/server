CC=g++
NAME=unified_logger
SOURCES=$(NAME).cpp $(NAME).h unified_logger_demo.cpp

all: logger

logger:
	$(CC) -g -o $(NAME) $(SOURCES) -std=c++11 -Wall -Wextra

test: all
	./$(NAME)

clean:
	rm -f $(NAME)