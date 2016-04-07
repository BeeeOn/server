CC=g++
NAME=unified_logger
SOURCES=$(wildcard *.cpp)
CFLAGS=-std=c++11 -Wall -Wextra

all: logger

logger:
	$(CC) -g -o $(NAME) $(SOURCES) -pthread $(CFLAGS)

run: all
	./$(NAME)

clean:
	rm -f $(NAME)

.PHONY: run clean