########################################################
## Makefile Notificator, IoT, FIT VUT Brno
## Author: Martin Doudera, xdoude00@stud.fit.vutbr.cz
## Date: 	 12/2014
########################################################

CC=g++
CFLAGS=-Wall -std=c++98
LDLIBS=-lcurl
DEPS=Notification.h
SOURCES= AchievementNotif.cpp SensorLowSignalNotif.cpp SensorLowBatteryNotif.cpp AdapterOfflineNotif.cpp XmlHelper.cpp AdapterAddedNotif.cpp SensorAddedNotif.cpp UriNotif.cpp DeleteNotif.cpp AdvertNotification.cpp InfoNotification.cpp AlertNotification.cpp Notificator.cpp JsonNotificationBuilder.cpp BaseNotification.cpp JsonUtil.cpp ControlNotification.cpp WatchdogNotif.cpp Utils.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXEC=main

all: libnotif.a libnotif.so

#VYTVORENI KNIHOVNY
#staticka knihovna
libnotif.a: $(OBJECTS)
	rm -f libnotif.a
	ar rcs libnotif.a $(OBJECTS)  

#dynamicka knihovna
libnotif.so: $(OBJECTS)
	rm -f libnotif.so
	g++ $(CFLAGS) -Wl,-soname,libnotif.so -shared $(OBJECTS) -o libnotif.so

#$(EXEC): $(OBJECTS) 
#		$(CC) $(LDLIBS) $(OBJECTS) -o $@

# OBJEKTY
.cpp.o:$(DEPS)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

#######################################################
# MAIN EXAMPLE

#main:  main.cpp
#	$(CC) $(CFLAGS) -static main.cpp -L. -lnotif -o main

main: all main.cpp
	$(CC) $(CFLAGS) main.cpp -o main -L. -lnotif -lcurl

########################################################

run: libnotif.so main
	./execute

clean:
	rm -f *.o libnotif.a libnotif.so  main
