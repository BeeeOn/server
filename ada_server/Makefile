CPPFLAGS= -Wall -W -std=c++11 -pthread -g
BUILDER= g++
ADA_SERVER= adaServer.cpp
SOURCE_FILES= pugixml.cpp loger.cpp config.cpp worker.cpp workerPool.cpp DBHandler.cpp connectionServer.cpp connectionHandler.cpp messageParsers.cpp sender.cpp listener.cpp requestServer.cpp messageCreator.cpp adaServerReceiver.cpp adaServerSender.cpp

all:
	make ada_server

ada_server: $(ADA_SERVER) $(SOURCE_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ -I/usr/local/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/include/postgresql/ -lpq 

run:
	./run_ada_server.sh

memtest:
	valgrind --tool=memcheck --trace-children=yes --leak-check=full --show-reachable=yes ./run_ada_server.sh

clean:
	rm ada_server
	
cleanlogs :
		rm *.log


