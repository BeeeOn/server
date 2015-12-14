CPPFLAGS= -Wall -W -std=c++11 -pthread -g
BUILDER= /usr/bin/g++
COMMON_FILES= 
LIBS_NEEDED= -I/usr/include/soci -L/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -I/usr/pgsql-9.2/include -I../ -L/usr/pgsql-9.2/lib -lpq -L../  -L../.. -L../../Notificator  -lnotif -lcurl


refresh_checker:refresh_checker.cpp $(COMMON_FILES) 
	$(BUILDER) $(CPPFLAGS) -o $@ $^ $(LIBS_NEEDED)
	
run:
	./refresh_checker


