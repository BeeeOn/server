CPPFLAGS= -Wall -W -std=c++11 -pthread -g
BUILDER= /usr/bin/g++
ADA_SERVER= adaServer.cpp
METHOD_TEST= methodTester.cpp
SOURCE_FILES= ../lib/pugixml.cpp loger.cpp config.cpp SSLContainer.cpp worker.cpp workerPool.cpp DBHandler.cpp connectionServer.cpp connectionHandler.cpp messageParsers.cpp sender.cpp listener.cpp requestServer.cpp messageCreator.cpp adaServerReceiver.cpp adaServerSender.cpp
OBJS = pugi.o loger.o config.o SSLContainer.o worker.o workerPool.o DBHandler.o messageCreator.o connectionServer.o connectionHandler.o messageParsers.o sender.o listener.o requestServer.o adaServerReceiver.o adaServerSender.o adaServer.o
LINKS= -L/usr/local/lib -lssl -lcurl -lcrypto -I/usr/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/pgsql-9.2/include -L/usr/pgsql-9.2/lib -lpq -Wl,-rpath,/usr/local/lib

all:
	make ada_server

ada_server: $(ADA_SERVER) $(OBJS)
	$(BUILDER) $(CPPFLAGS) $(OBJS) -o $@ -g $(LINKS)
	
pugi.o:
	$(BUILDER) $(CPPFLAGS) -c ../lib/pugixml.cpp -o $@
	
loger.o:
	$(BUILDER) $(CPPFLAGS) -c loger.cpp -o $@
	
config.o:
	$(BUILDER) $(CPPFLAGS) -c config.cpp -o $@
SSLContainer.o:
	$(BUILDER) $(CPPFLAGS) -c SSLContainer.cpp -o $@ $(LINKS)
worker.o: 
	$(BUILDER) $(CPPFLAGS) -c worker.cpp -o $@ $(LINKS)
workerPool.o:
	$(BUILDER) $(CPPFLAGS) -c workerPool.cpp -o $@ $(LINKS)
DBHandler.o:
	$(BUILDER) $(CPPFLAGS) -c DBHandler.cpp -o $@ $(LINKS)
messageCreator.o:
	$(BUILDER) $(CPPFLAGS) -c messageCreator.cpp -o $@ $(LINKS)
connectionServer.o:
	$(BUILDER) $(CPPFLAGS) -c connectionServer.cpp -o $@ $(LINKS)
connectionHandler.o:
	$(BUILDER) $(CPPFLAGS) -c connectionHandler.cpp -o $@ $(LINKS)
messageParsers.o:
	$(BUILDER) $(CPPFLAGS) -c messageParsers.cpp -o $@ $(LINKS)
sender.o:
	$(BUILDER) $(CPPFLAGS) -c sender.cpp -o $@ $(LINKS)
listener.o:
	$(BUILDER) $(CPPFLAGS) -c listener.cpp -o $@ $(LINKS)
requestServer.o:
	$(BUILDER) $(CPPFLAGS) -c requestServer.cpp -o $@ $(LINKS)
adaServerReceiver.o:
	$(BUILDER) $(CPPFLAGS) -c adaServerReceiver.cpp -o $@ $(LINKS)
adaServerSender.o:
	$(BUILDER) $(CPPFLAGS) -c adaServerSender.cpp -o $@ $(LINKS)
adaServer.o:
	$(BUILDER) $(CPPFLAGS) -c adaServer.cpp -o $@ $(LINKS)
	
ada_old: $(ADA_SERVER) $(SOURCE_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ -g -L/usr/local/lib -lssl -lcurl -lcrypto -I/usr/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/pgsql-9.2/include -L/usr/pgsql-9.2/lib -lpq -Wl,-rpath,/usr/local/lib

method_test: $(METHOD_TEST) $(SOURCE_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^  -g -L/usr/local/lib -lssl -lcurl -lcrypto -I/usr/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -L/usr/pgsql-9.2/lib -I/usr/local/pgsql/include -lpq 

run:
	./run_ada_server.sh

memtest:
	valgrind --tool=memcheck --trace-children=yes --leak-check=full --show-reachable=yes ./run_ada_server.sh

clean:
	rm ada_server
	rm *.o
	
cleanlogs:
		rm *.log

rebuild: clean all
