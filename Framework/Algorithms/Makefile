CPPFLAGS= -Wall -W -std=c++11 -pthread -g
BUILDER= /opt/centos/devtoolset-1.1/root/usr/bin/g++
COMMON_FILES= ../../pugixml.cpp ../algorithm.cpp ../../databaseConnectionContainer.cpp ../DBFWHandler.cpp ../../loger.cpp
LIBS_NEEDED= -I/usr/local/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/pgsql-9.2/include -L/usr/pgsql-9.2/lib -lpq -L../  -lnotif -lcurl

WATCHNOTIF= watch_and_notify.cpp

all:
	make watch_and_notify
	make geofencing
	
example:example.cpp $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ $(LIBS_NEEDED)
	
watch_and_notify:watch_and_notify.cpp $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ $(LIBS_NEEDED) 
	
geofencing:geofencing.cpp $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ $(LIBS_NEEDED)



