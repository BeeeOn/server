CC=g++
NAME=unified_logger
SOURCES=$(NAME).cpp $(NAME).h unified_logger_demo.cpp utility.cpp

all: logger

logger:
	$(CC) -g -o $(NAME) $(SOURCES) -pthread -std=c++11 -Wall -Wextra

test: all
	./$(NAME)

clean:
	rm -f $(NAME)