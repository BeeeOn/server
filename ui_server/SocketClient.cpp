/* 
 * File:   SocketClient.cpp
 * Author: pavel
 * 
 * Created on 1. srpen 2014, 9:43
 */

#include <string>

#include "SocketClient.h"
using namespace std;

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
    int n;
    n = ::write(_socketfd, text.c_str(), text.length());
    if (n < 0) 
        throw "ERROR writing to socket";
}

string SocketClient::read(){

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0; 
    setsockopt(_socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
    
    

    string data;
    int recieved;
    int bufferSize = 1024;
    char rc[bufferSize];
    while(1)
     {
        bzero(rc,bufferSize);
        //recieved = recv(_socketfd, rc, bufferSize,0);
         recieved = ::read(_socketfd,rc,bufferSize);
                //printf("Bytes received: %d\n",recieved);
                if ( recieved > 0 )
                {
                     data.append(rc, recieved);
                      if ( (data.find("</reply>")!=std::string::npos)||(data.find("</com>")!=std::string::npos)||((data[data.size()-2]=='/')&&(data[data.size()-1]=='>'))) 
                            break; 
                }
                else if ( recieved == 0 )
                {
                       if(data.length()>0)
                       {
                           break;
                       }
                       else{
                            throw "ERROR reading from socket";
                             free(rc);
                       }
                       break;
                }
                else{
                    throw "ERROR reading from socket";
                }
    }
    return data;
}

