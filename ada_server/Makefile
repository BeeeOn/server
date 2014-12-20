CPPFLAGS= -Wall -W -std=c++11 -pthread -g
BUILDER= /opt/centos/devtoolset-1.1/root/usr/bin/g++
ADA_SERVER_RECIVER= ada_server_receiver.cpp
ADA_SERVER_SENDER= ada_server_sender.cpp
COMMON_FILES= pugixml.cpp loger.cpp config.cpp databaseConnectionContainer.cpp DBHandler.cpp

all:
	make ada_server_receiver
	make ada_server_sender

ada_server_receiver: $(ADA_SERVER_RECIVER) $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ -I/usr/local/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/include/postgresql/ -lpq -L./notif -lnotif -lcurl

ada_server_sender: $(ADA_SERVER_SENDER) $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ -I/usr/local/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/include/postgresql/ -lpq

run:
	./run_ada_server.sh

memtest:
	valgrind --tool=memcheck --trace-children=yes --leak-check=full --show-reachable=yes ./run_server.sh

clean:
	rm ada_server_receiver
	rm ada_server_sender


