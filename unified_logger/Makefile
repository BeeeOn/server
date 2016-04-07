CC=g++
NAME=unified_logger
SOURCES=$(wildcard *.cpp)
CFLAGS=-std=c++11 -Wall -Wextra

all: logger

logger:
	$(CC) -g -o $(NAME) $(SOURCES) -pthread $(CFLAGS)

run: all
	./$(NAME)
	@echo "Log files tail:"
	@tail -n 5 *.log

clean:
	rm -f $(NAME)
	rm -f *.log

.PHONY: run clean