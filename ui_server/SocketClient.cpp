/* 
 * File:   SocketClient.cpp
 * Author: pavel
 * 
 * Created on 1. srpen 2014, 9:43
 */

#include <string>

#include "SocketClient.h"


SocketClient::SocketClient(int portNumber, string hostName) {
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    _socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketfd < 0) 
        throw "ERROR opening socket";
    server = gethostbyname(hostName.c_str());
    if (server == NULL) {
        throw "ERROR, no such host\n";
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
            (char *)&serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(portNumber);
    if (connect(_socketfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        throw "ERROR connecting";
    
}

SocketClient::~SocketClient() {
    close(_socketfd);
}


void SocketClient::write(string text){
    char buffer[256];
    int n;
    
    bzero(buffer,256);
    n = ::write(_socketfd, text.c_str(), text.length());
    if (n < 0) 
        throw "ERROR writing to socket";
}

string SocketClient::read(){
    int n;
    char buffer[256];
    
    bzero(buffer,256);
    
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0; 
    setsockopt(_socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
    
    n = ::read(_socketfd,buffer,255);
    if (n < 0) 
        throw "ERROR reading from socket";
    return buffer;
}