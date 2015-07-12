/* 
 * File:   SocketServer.cpp
 * Author: pavel
 * 
 * Created on 31. březen 2015, 22:49
 */

#include "SocketServer.h"
#include<stdio.h>
#include<string.h>    //strlen
#include<string>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include <iostream>
#include<unistd.h>    //write

SocketServer::SocketServer() {
    _port = -1;
}


SocketServer::~SocketServer() {
}

int SocketServer::start(int port){
    _port = port;
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( _port );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
     
    //Receive a message from client
    //while( 1 )
    {
        char client_message[2000];
        bzero(client_message,2000);
        //Send the message back to client
        read_size = recv(client_sock , client_message , 2000 , 0);
        
        std::string reply="server reply: ";
        reply.append(client_message);
        std::cout<<"reply:" << reply<<std::endl;
        write(client_sock , reply.c_str()  , reply.length());
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    std::cout<< "serverdone"<<std::endl;
    return 0;
}

