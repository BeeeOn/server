###Makefile demo applications
clienttemplate=../../client.cpp

all: s_talkative_bunch s_single s_timeout
#client

s_single: s_single.cpp $(clienttemplate)
	g++ $(clienttemplate) s_single.cpp -g -o s_single -std=c++11 -pthread -lssl -lcrypto

s_talkative_bunch: s_talkative_bunch.cpp $(clienttemplate)
	g++  $(clienttemplate) s_talkative_bunch.cpp -g -o s_talkative_bunch -std=c++11 -pthread -lssl -lcrypto

s_timeout: s_timeout.cpp $(clienttemplate)
	g++ $(clienttemplate) s_timeout.cpp -g -o s_timeout -std=c++11 -pthread -lssl -lcrypto

clean:
	rm -f s_talkative_bunch s_single s_timeout

