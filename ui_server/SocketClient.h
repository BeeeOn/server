/* 
 * File:   SocketClient.h
 * Author: pavel
 *
 * Created on 1. srpen 2014, 9:43
 */

#ifndef SOCKETCLIENT_H
#define	SOCKETCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

using namespace std;

class SocketClient {

public:
    
    SocketClient(int portNumber, string hostName="localhost");
    virtual ~SocketClient();
    void write(string text);
    string read();
private:
    int _socketfd;
    
};

#endif	/* SOCKETCLIENT_H */

