CPPFLAGS= -Wall -W -std=c++11 -pthread -g
BUILDER= /opt/centos/devtoolset-1.1/root/usr/bin/g++
COMMON_FILES= ../../pugixml.cpp ../algorithm.cpp

WATCHNOTIF= watch_and_notify.cpp

all:
	echo "use make name_of_algorithm"
	
example:example.cpp $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^  -I/usr/local/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/include/postgresql/ -lpq -L../  -lnotif -lcurl
	
watch_and_notify:watch_and_notify.cpp $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^  -I/usr/local/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/include/postgresql/ -lpq -L../  -lnotif -lcurl



