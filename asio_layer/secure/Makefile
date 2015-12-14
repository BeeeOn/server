###Makefile secure server & client

all: server 
#client

reserver: server.cpp session.cpp
	rm -f server
	g++ server.cpp session.cpp -g -o server -std=c++11 -pthread -lssl -lcrypto -lboost_system -lboost_log

server: server.cpp session.cpp
	g++ server.cpp session.cpp -g -o server -std=c++11 -pthread -lssl -lcrypto -lboost_system -lboost_log

#client: client.cpp 
#	g++ client.cpp -g -o client -std=c++11 -pthread -lssl -lcrypto -lboost_system -lboost_log


#g++ -std=c++11 -DBOOST_LOG_DYN_LINK -c boosttest.cc 
#g++ boosttest.o -lboost_log -lpthread -o boosttest

clean:
	rm -f server
	rm -f client

memtest: server
	valgrind --leak-check=full ./server 8001
	
