CPPFLAGS= -Wall -W -std=c++11 -pthread -g
BUILDER= /opt/centos/devtoolset-1.1/root/usr/bin/g++
FRAMEWORK= framework.cpp
COMMON_FILES= ../pugixml.cpp ../loger.cpp ../config.cpp ../databaseConnectionContainer.cpp DBFWHandler.cpp

all:
	make framework

framework: $(FRAMEWORK) $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ -I/usr/local/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/include/postgresql/ -lpq -L.  -lnotif -lcurl

run:
	./run_framework.sh

memtest:
	valgrind --tool=memcheck --trace-children=yes --leak-check=full --show-reachable=yes ./run_framework.sh	
	
testAdapter: testAdapter.cpp
	$(BUILDER) $(CPPFLAGS) -o $@ $^ 
	
testUIServer: testUIServer.cpp
	$(BUILDER) $(CPPFLAGS) -o $@ $^ 
	
testUIServerGetAllAlgs: testUIServerGetAllAlgs.cpp
	$(BUILDER) $(CPPFLAGS) -o $@ $^ 	
	
testUIServerGetAlgs: testUIServerGetAlgs.cpp
	$(BUILDER) $(CPPFLAGS) -o $@ $^ 
	
testUIServerDelAlg: testUIServerDelAlg.cpp
	$(BUILDER) $(CPPFLAGS) -o $@ $^ 
	
clean:
	rm framework
	


