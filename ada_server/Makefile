CXX= g++
CC = g++

CXXFLAGS= -Wall -W -std=c++11 -pthread -g
CXXFLAGS += -I/usr/pgsql-9.2/include
CXXFLAGS += -I/usr/include/soci

LDFLAGS += -L/usr/local/lib
LDFLAGS += -L/usr/pgsql-9.2/lib
LDFLAGS += -Wl,-rpath,/usr/local/lib

METHOD_TEST= methodTester.cpp

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
ada_old: $(ADA_SERVER) $(SOURCE_FILES) 
	$(CXX) $(CXXFLAGS) -o $@ $^ -g -L/usr/local/lib -lssl -lcurl -lcrypto -I/usr/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/pgsql-9.2/include -L/usr/pgsql-9.2/lib -lpq -Wl,-rpath,/usr/local/lib

method_test: $(METHOD_TEST) $(SOURCE_FILES) 
	$(CXX) $(CXXFLAGS) -o $@ $^  -g -L/usr/local/lib -lssl -lcurl -lcrypto -I/usr/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -L/usr/pgsql-9.2/lib -I/usr/local/pgsql/include -lpq 

run:
	./run_ada_server.sh

memtest:
	valgrind --tool=memcheck --trace-children=yes --leak-check=full --show-reachable=yes ./run_ada_server.sh

clean:
	$(RM) ada_server
	$(RM) adaServer.o $(OBJS)
	
cleanlogs:
		$(RM) *.log

rebuild: clean all

.PHONY: all run memtest clean cleanlogs rebuild
