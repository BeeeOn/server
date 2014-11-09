CPPFLAGS= -Wall -W -std=c++11 -pthread -g
ADA_SERVER_RECIVER= ada_server_receiver.cpp
COMMON_FILES= pugixml.cpp loger.cpp config.cpp databaseConnectionContainer.cpp DBHandler.cpp

all:
	make ada_server_receiver
	make ada_server_sender

ada_server_receiver: $(ADA_SERVER_RECIVER) $(COMMON_FILES) 
	/opt/centos/devtoolset-1.1/root/usr/bin/g++ $(CPPFLAGS) -o $@ $^ -I/usr/local/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/include/postgresql/ -lpq

ada_server_sender: ada_server_sender.cpp  pugixml.cpp loger.cpp config.cpp
	/opt/centos/devtoolset-1.1/root/usr/bin/g++ $(CPPFLAGS) -o $@ $^ -I/usr/local/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/include/postgresql/ -lpq

run:
	./run_ada_server.sh

memtest:
	valgrind --tool=memcheck --trace-children=yes --leak-check=full --show-reachable=yes ./run_server.sh

clean:
	rm ada_server_receiver
	rm ada_server_sender


