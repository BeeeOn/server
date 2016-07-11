CXXFLAGS= -Wall -W -std=c++11 -pthread -g
CXX= g++
ADA_SERVER= adaServer.cpp
METHOD_TEST= methodTester.cpp
SOURCE_FILES += ../lib/pugixml.cpp
SOURCE_FILES += loger.cpp
SOURCE_FILES += config.cpp
SOURCE_FILES += SSLContainer.cpp
SOURCE_FILES += worker.cpp
SOURCE_FILES += workerPool.cpp
SOURCE_FILES += DBHandler.cpp
SOURCE_FILES += connectionServer.cpp
SOURCE_FILES += connectionHandler.cpp
SOURCE_FILES += messageParsers.cpp
SOURCE_FILES += sender.cpp
SOURCE_FILES += listener.cpp
SOURCE_FILES += requestServer.cpp
SOURCE_FILES += messageCreator.cpp
SOURCE_FILES += adaServerReceiver.cpp
SOURCE_FILES += adaServerSender.cpp
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
LINKS += -L/usr/local/lib
LINKS += -lssl
LINKS += -lcurl
LINKS += -lcrypto
LINKS += -I/usr/include/soci
LINKS += -L/usr/local/lib64
LINKS += -lsoci_core
LINKS += -lsoci_empty
LINKS += -lsoci_postgresql
LINKS += -ldl
LINKS += -I/usr/pgsql-9.2/include
LINKS += -L/usr/pgsql-9.2/lib
LINKS += -lpq
LINKS += -Wl,-rpath,/usr/local/lib

all:
	make ada_server

ada_server: $(ADA_SERVER) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ -g $(LINKS)
	
pugi.o:
	$(CXX) $(CXXFLAGS) -c ../lib/pugixml.cpp -o $@
	
loger.o:
	$(CXX) $(CXXFLAGS) -c loger.cpp -o $@
	
config.o:
	$(CXX) $(CXXFLAGS) -c config.cpp -o $@
SSLContainer.o:
	$(CXX) $(CXXFLAGS) -c SSLContainer.cpp -o $@ $(LINKS)
worker.o: 
	$(CXX) $(CXXFLAGS) -c worker.cpp -o $@ $(LINKS)
workerPool.o:
	$(CXX) $(CXXFLAGS) -c workerPool.cpp -o $@ $(LINKS)
DBHandler.o:
	$(CXX) $(CXXFLAGS) -c DBHandler.cpp -o $@ $(LINKS)
messageCreator.o:
	$(CXX) $(CXXFLAGS) -c messageCreator.cpp -o $@ $(LINKS)
connectionServer.o:
	$(CXX) $(CXXFLAGS) -c connectionServer.cpp -o $@ $(LINKS)
connectionHandler.o:
	$(CXX) $(CXXFLAGS) -c connectionHandler.cpp -o $@ $(LINKS)
messageParsers.o:
	$(CXX) $(CXXFLAGS) -c messageParsers.cpp -o $@ $(LINKS)
sender.o:
	$(CXX) $(CXXFLAGS) -c sender.cpp -o $@ $(LINKS)
listener.o:
	$(CXX) $(CXXFLAGS) -c listener.cpp -o $@ $(LINKS)
requestServer.o:
	$(CXX) $(CXXFLAGS) -c requestServer.cpp -o $@ $(LINKS)
adaServerReceiver.o:
	$(CXX) $(CXXFLAGS) -c adaServerReceiver.cpp -o $@ $(LINKS)
adaServerSender.o:
	$(CXX) $(CXXFLAGS) -c adaServerSender.cpp -o $@ $(LINKS)
adaServer.o:
	$(CXX) $(CXXFLAGS) -c adaServer.cpp -o $@ $(LINKS)
	
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
	$(RM) *.o
	
cleanlogs:
		$(RM) *.log

rebuild: clean all
