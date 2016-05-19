### Unified_logger Makefile
### Library used for logging in unified format in multi-thread applications
### @author Marek Beňo, xbenom01 at stud.fit.vutbr.cz
### 7. April 2016

CC=g++
NAME=unified_logger
SOURCES=$(wildcard *.cpp)
CFLAGS=-std=c++11 -Wall -Wextra

all: $(NAME)

$(NAME): $(SOURCES)
	$(CC) -g -o $(NAME) $(SOURCES) -pthread $(CFLAGS)

run: $(NAME)
	./$(NAME)
	@echo "Log files tail:"
	@tail -n 5 *.log

clean:
	rm -f $(NAME)
	rm -f *.log

.PHONY: run clean