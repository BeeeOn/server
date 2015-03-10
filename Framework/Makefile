CPPFLAGS= -Wall -W -std=c++11 -pthread -g
BUILDER= /opt/centos/devtoolset-1.1/root/usr/bin/g++
FW= framework.cpp
COMMON_FILES= ../pugixml.cpp ../loger.cpp ../config.cpp ../databaseConnectionContainer.cpp ../DBHandler.cpp
COMMON_FILES_WN= ../pugixml.cpp algorithm.cpp

WATCHNOTIF= watch_and_notify.cpp

all:
	make fw

fw: $(FW) $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ -I/usr/local/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/include/postgresql/ -lpq -L.  -lnotif -lcurl

testAdapter: testAdapter.cpp
	$(BUILDER) $(CPPFLAGS) -o $@ $^ 
clean:
	rm fw
	
alg1:$(WATCHNOTIF) $(COMMON_FILES_WN) 
	$(BUILDER) $(CPPFLAGS) -I boost_1_57_0 -o $@ $^  -I/usr/local/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/include/postgresql/ -lpq -L.  -lnotif -lcurl -lboost_program_options
	


