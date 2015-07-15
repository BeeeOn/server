CPPFLAGS= -Wall -W -std=c++11 -pthread -g
BUILDER= g++
COMMON_FILES= ../../lib/pugixml.cpp ../algorithm.cpp ../DBFWHandler.cpp ../loger.cpp ../DBConnectionsContainer.cpp
LIBS_NEEDED= -I/usr/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/pgsql-9.2/include -L/usr/pgsql-9.2/lib -lpq -L../  -lnotif -lcurl

WATCHNOTIF= watch_and_notify.cpp

all:
	make watch_and_notify
	make geofencing
	
example:example.cpp $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ $(LIBS_NEEDED)
	
watch_and_notify:watch_and_notify.cpp $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ $(LIBS_NEEDED) 
	
test:watch_and_notify.cpp $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ $(LIBS_NEEDED)
	
geofencing:geofencing.cpp $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ $(LIBS_NEEDED)

wdebug:
	$(BUILDER) $(CPPFLAGS) -o $@ $^ $(LIBS_NEEDED)



