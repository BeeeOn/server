#include <string>
#include <stdio.h>
#include <string.h>  
#include <iostream>
#include <unistd.h>
#include <errno.h>

#include "SocketClient.h"

using namespace std;

SocketClient::SocketClient(int portNumber, string hostName) {
   
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    _socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketfd < 0) 
        throw SocketClientException("ERROR opening socket");
    server = gethostbyname(hostName.c_str());
    if (server == NULL) {
        throw SocketClientException("ERROR, no such host\n");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
            (char *)&serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(portNumber);
    if (connect(_socketfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        throw SocketClientException("ERROR connecting");
    
}

SocketClient::~SocketClient() {
    close(_socketfd);
}

int SocketClient::write(string text){
    int n;
    n = ::write(_socketfd, text.c_str(), text.length());
    if (n < 0) 
        return 0;
    return 1;
}

string SocketClient::read(){

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0; 
    setsockopt(_socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
    
    string data;
    int bufferSize = 1000;
    char rc[bufferSize+2];
    while(1)
    {
        bzero(rc,bufferSize+1);
        int recieved = ::read(_socketfd,rc,bufferSize);
         
        if ( recieved > 0 ) {
            rc[recieved] = '\0';
            data.append(rc, strlen(rc));
            
            if ((data.find("</reply>")!=std::string::npos) ||
                (data.find("</com>")!=std::string::npos) ||
                ((data[data.size()-2]=='/')&&(data[data.size()-1]=='>'))
                ) 
                break; 
        } else if ( recieved == 0 ) {
            if(data.length()>0)
                break;   
            else
                throw SocketClientException("ERROR reading from socket");
            
            break;
        } else {
            throw SocketClientException("ERROR reading from socket");
        }
    }
    
    // Client done reading;
    return data;
}

// Check if the string ends with given ending
bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {

        // Copy the string in order to remove endline from the end
        string s = fullString;                                                                                                     
        
        // Remove endline from the end of the string
        if (s[s.length()-1] == '\n') {                                                                                             
            s.erase(s.length()-1);                                                                                             
        }
        
        // Compare last chars with endTag
        int res = s.compare (s.length() - ending.length(), ending.length(), ending);                                               
        
        return (0 == res); 
    } else {
        return false;
    }
}

string SocketClient::readUntilendTag(string endTag) {
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0; 
    setsockopt(_socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
    
    string data;
    
    int bufferSize = 1000;
    char rc[bufferSize+1];
    while(1)
     {
        bzero(rc,bufferSize+1);
        //recieved = recv(_socketfd, rc, bufferSize,0);
        int recieved;
         recieved = ::read(_socketfd,rc,bufferSize);
         
         
         
                 //   std::cout<<"|"<<rc<<"|"<<endl;
                //printf("Bytes received: %d vs %d\n",recieved,strlen(rc));
                if ( recieved > 0 )
                {
                    rc[recieved] = '\0';
                    
                    //std::cout << "data before append" << data << "|" << strlen(rc) << std::endl;           
                    data.append(rc, strlen(rc));
                    //std::cout << "after" << data << "|" << std::endl;
                      //if ( data.find(endTag)!=std::string::npos ) 
                    if(hasEnding(data, endTag))
                        break; 
                }
                else if ( recieved == 0 )//other side closed socket
                {
                    if(data.length()>0)
                    {
                        break;
                    }
                    else{
                        throw SocketClientException("ERROR reading from socket, zero length");
                    }
                    break;
                }
                else{
                    throw SocketClientException("ERROR reading from socket, recieved code:" + to_string((long long int)recieved) + " errno: " + strerror(errno));
                }
    }
    std::cout<<"client done reading"<< data <<endl;
    return data;
}

SocketClientException::SocketClientException(string errText) : runtime_error(errText), _errCode(1),_errText(errText) {
}

const char* SocketClientException::what() const throw()
{
    return _errText.c_str();
}
