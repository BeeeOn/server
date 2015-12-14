
###Makefile demo applications: server
servertemplate=../../server.cpp
sessiontemplate=../../session.cpp

all: s_classic

s_classic: s_classic.cpp $(servertemplate) $(sessiontemplate)
	g++ $(servertemplate) $(sessiontemplate) s_classic.cpp -g -o s_classic -std=c++11 -pthread -lssl -lcrypto

clean:
	rm -f s_classic

#memtest: server
#	valgrind --leak-check=full ./server 8001
	