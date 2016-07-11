CXX= g++
CC = g++

CXXFLAGS= -Wall -W -std=c++11 -pthread -g
CXXFLAGS += -I/usr/pgsql-9.2/include
CXXFLAGS += -I/usr/include/soci

LDFLAGS += -L/usr/local/lib
LDFLAGS += -L/usr/pgsql-9.2/lib
LDFLAGS += -Wl,-rpath,/usr/local/lib

OBJS += pugi.o
OBJS += loger.o
OBJS += config.o
OBJS += SSLContainer.o
OBJS += worker.o
OBJS += workerPool.o
OBJS += DBHandler.o
OBJS += messageCreator.o
OBJS += connectionServer.o
OBJS += connectionHandler.o
OBJS += messageParsers.o
OBJS += sender.o
OBJS += listener.o
OBJS += requestServer.o
OBJS += adaServerReceiver.o
OBJS += adaServerSender.o
OBJS += adaServer.o

LDLIBS += -lpthread
LDLIBS += -lssl
LDLIBS += -lcurl
LDLIBS += -lcrypto
LDLIBS += -lsoci_core
LDLIBS += -lsoci_empty
LDLIBS += -lsoci_postgresql
LDLIBS += -ldl
LDLIBS += -lpq

all: ada_server

ada_server: adaServer
	cp $< $@
adaServer: adaServer.o $(OBJS)

pugi.o: ../lib/pugixml.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

memtest:
	valgrind --tool=memcheck --trace-children=yes --leak-check=full --show-reachable=yes ./run_ada_server.sh

clean:
	$(RM) ada_server
	$(RM) adaServer.o $(OBJS)
	
cleanlogs:
	$(RM) *.log

rebuild: clean all

.PHONY: all memtest clean cleanlogs rebuild
