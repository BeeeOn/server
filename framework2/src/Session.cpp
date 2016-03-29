/* 
 * File:   Session.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 15. March 2016
 */

#include "Session.h"

#include <stdexcept>

Session::Session(asio::io_service& io_service):
    m_socket(io_service)
{
}

Session::~Session() {
    try {
        if (m_socket.is_open()) {
            m_socket.close();            
        }
    }
    catch (asio::system_error const& e) {
        
        std::cerr << "Session::stop exception: " << e.what() << std::endl;
    }
    std::cerr << "Session::~Sessions - session stopped." << std::endl;
}


asio::ip::tcp::socket& Session::getSocket() {
    return m_socket;
}

void Session::onStart()
{
    async_read(m_socket, m_buffer,
                boost::bind(&Session::handleRead, shared_from_this(),
                asio::placeholders::error, asio::placeholders::bytes_transferred));


    /*
    m_socket.async_receive(m_buffer, 0,
                boost::bind(&Session::handleRead, shared_from_this(),
                asio::placeholders::error, asio::placeholders::bytes_transferred));

     */
}

std::string Session::convertMessage(int msg_length) {
    
    asio::streambuf::const_buffers_type bufstream = m_buffer.data();
    std::string message(asio::buffers_begin(bufstream), asio::buffers_begin(bufstream) + msg_length);
    
    // Clear buffer.
    m_buffer.consume(msg_length);
    return message;
}

void Session::handleRead(const asio::error_code& error, size_t bytes_transferred)
{
    receivedMessage(bytes_transferred);
    
    
    /*
    if (!error) {
      
        //bytes_transferred_ = bytes_transferred;
        onRead(bytes_transferred);
    }
    else  //shortread
    {
        std::cerr << "Bytes: " << bytes_transferred << " handle_read: " << error.message() << std::endl;
      //delete this;
    }
     */
}

void Session::handleWrite(const asio::error_code& error)
{
    if (!error) {
        std::cout << "write was successful" << std::endl;
    }
    else {
        std::cerr << "handle_write: " << error.message() << std::endl;
    }
}

void Session::send(std::string message)
{
    size_t message_length = message.length();
    
    asio::async_write(m_socket, asio::buffer(message, message_length),
          boost::bind(&Session::handleWrite, shared_from_this(), asio::placeholders::error));        
}