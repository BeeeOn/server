#ifndef SOCKETCLIENT_H
#define	SOCKETCLIENT_H
#include <exception>
#include <stdexcept>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

class SocketClient {

public:
    
    SocketClient(int portNumber, std::string hostName="localhost");
    virtual ~SocketClient();
    int write(std::string text);
    std::string read();
    std::string readUntilendTag(std::string endTag);
private:
    int _socketfd;
    
};

class SocketClientException : public std::runtime_error {
public:
    //SocketClientException(int errCode);
    SocketClientException(std::string errText);
   // ServerException(int errCode, string errText, string id, string state);
   // virtual ~ServerException() throw();
    virtual ~SocketClientException() throw() {};
    virtual const char* what() const throw();
private:
    int _errCode;
    std::string _errText;
};


#endif	/* SOCKETCLIENT_H */

