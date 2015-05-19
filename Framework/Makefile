CPPFLAGS= -Wall -W -std=c++11 -pthread -g
BUILDER= /opt/centos/devtoolset-1.1/root/usr/bin/g++
FRAMEWORK= framework.cpp
COMMON_FILES= handleAdapterMessage.cpp handleAlgorithmMessage.cpp handleUIServerMessage.cpp config.cpp pugixml.cpp loger.cpp DBConnectionsContainer.cpp DBFWHandler.cpp AdapterMessageParser.cpp
LIBS_NEEDED= -I/usr/local/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/pgsql-9.2/include -L/usr/pgsql-9.2/lib -lpq -L. -lnotif -lcurl

.PHONY: all run clean

all:
	make framework

framework: $(FRAMEWORK) $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ $(LIBS_NEEDED)

run:
	./run_framework.sh &

testUIServer: testUIServer.cpp
	$(BUILDER) $(CPPFLAGS) -o $@ $^
	
testAdapter: testAdapter.cpp
	$(BUILDER) $(CPPFLAGS) -o $@ $^
	
unitTest: test.cpp
	$(BUILDER) $(CPPFLAGS) -o $@ $^

memtest:
	valgrind --tool=memcheck --trace-children=yes --leak-check=full --show-reachable=yes ./run_framework.sh	
	
clean:
	rm framework

doc:
	doxygen Doxyfile
	


