/* 
 * File:   GatewayInterface.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 13. April 2016
 */

#ifndef GATEWAYINTERFACE_H
#define GATEWAYINTERFACE_H

#include <string>

#include <asio.hpp>
#include <sstream>
#include <string>

#define ADA_SERVER_SENDER_PORT 7081

class GatewayInterface
{
public:
    GatewayInterface();
    
    virtual ~GatewayInterface();
    
    void connect();
    
    int sendPingGateway(long long gateway_id);
    
    void sendSetState(long long gateway_id, long device_euid, int module_id, std::string new_value);
    
    //std::string convertData(int reply_length);

    //void onStart();
    //void onWrite();
    //void onRead(); 
    
    void send();
    
    void receive();
    
    std::string m_response;
    
private:
    
    void handleConnect(const asio::error_code& error);
    
    void handleWrite(const asio::error_code& error,
        size_t bytes_transferred);
    
    void handleRead(const asio::error_code& error,
        size_t bytes_transferred);    
    
    asio::io_service m_io_service;
    
    asio::ip::tcp::socket m_socket;
    
    asio::ip::tcp::resolver m_resolver;
    
    asio::streambuf m_reply_buffer;
    
    std::string m_request;
    
    int m_port;
    /*
    std::string m_request;
 
    std::string m_reply;
    
    size_t m_bytes_transferred;
    
    */
};

#endif /* GATEWAYINTERFACE_H */

