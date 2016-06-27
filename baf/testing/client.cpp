/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: mrmaidx
 *
 * Created on 13. dubna 2016, 22:22
 */

#include <cstdlib>
#include <asio.hpp>
#include <iostream>

int main(int argc, char** argv) {

    //GatewayInterface gi;
    
    //int response = gi.sendPingGateway(123);
    

    try
    {
    // Create input output service.
    asio::io_service io_service;

    // Create socket for connection.
    asio::ip::tcp::socket socket(io_service);
    
    // Connect to localhost and port of ada_server_sender.
    socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 7081));
    
    // Create a message to send.
    asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                      "<request type=\"switch\">"
                        "<sensor id=\"3925868550\" type=\"5\" onAdapter=\"1858610446240916\">"
                            "<value>30</value>"
                        "</sensor>"
                      "</request>" << std::endl;

    // Send the message.
    asio::write(socket, request);

    //asio::streambuf response;
    //asio::read_until(socket, response, "</reply>");

    //std::istream response_stream(&response);

    //std::cout << response_stream << std::endl;

  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }

  return 0;
}