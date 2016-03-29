/* 
 * File:   Session.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 15. March 2016
 */

#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <string>

#include <boost/bind.hpp>
#include <asio.hpp>

class Session: public std::enable_shared_from_this<Session>
{
public:
    Session(asio::io_service& io_service);
    
    virtual ~Session();
    
    asio::ip::tcp::socket& getSocket();
  
    std::string convertMessage(int msg_length);
  
    void onStart();
  
    virtual void receivedMessage(size_t bytes_transferred) = 0;
 
protected:
    
    void handleRead(const asio::error_code& error, size_t bytes_transferred);

    void handleWrite(const asio::error_code& error);
    
    void send(std::string message);
    
    //enum { max_length = 1024 };
    
    asio::ip::tcp::socket m_socket;
    
    //char data_[max_length];
    
    //std::string response_;
    
    asio::streambuf m_buffer;

};

#endif /* SESSION_H */