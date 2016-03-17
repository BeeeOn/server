CPPFLAGS= -Wall -W -std=c++11 -pthread -g
BUILDER= /usr/bin/g++
ADA_SERVER= adaServer.cpp
METHOD_TEST= methodTester.cpp
SOURCE_FILES= ../lib/pugixml.cpp loger.cpp config.cpp SSLContainer.cpp worker.cpp workerPool.cpp DBHandler.cpp connectionServer.cpp connectionHandler.cpp messageParsers.cpp sender.cpp listener.cpp requestServer.cpp messageCreator.cpp adaServerReceiver.cpp adaServerSender.cpp

all:
	make ada_server

ada_server: $(ADA_SERVER) $(SOURCE_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ -g -L/usr/local/lib -lssl -lcurl -lcrypto -I/usr/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/pgsql-9.2/include -L/usr/pgsql-9.2/lib -lpq -Wl,-rpath,/usr/local/lib

method_test: $(METHOD_TEST) $(SOURCE_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^  -g -L/usr/local/lib -lssl -lcurl -lcrypto -I/usr/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -L/usr/pgsql-9.2/lib -I/usr/local/pgsql/include -lpq 

run:
	./run_ada_server.sh

memtest:
	valgrind --tool=memcheck --trace-children=yes --leak-check=full --show-reachable=yes ./run_ada_server.sh

clean:
	rm ada_server
	
cleanlogs:
		rm *.log

rebuild: clean all