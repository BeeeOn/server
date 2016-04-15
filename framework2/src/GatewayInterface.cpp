/* 
 * File:   GatewayInterface.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 13. April 2016
 */

#include "GatewayInterface.h"

#include <iostream>
#include <boost/bind.hpp>
#include <sstream>
#include <stdexcept>

#include "pugixml.hpp"

GatewayInterface::GatewayInterface():
    m_socket(m_io_service),
    m_resolver(m_io_service),
    m_port(ADA_SERVER_SENDER_PORT)
{
}

GatewayInterface::~GatewayInterface()
{
}

int GatewayInterface::sendPingGateway(long long gateway_id)
{
    std::cout << "GatewayInterface::sendPingGateway" << std::endl;
    /*
    <request type="ping">
    <adapter id="1234"/>
    </request>

    <reply errorCode="0">true</reply>
    <reply errorCode="X">false</reply>
    X = 1 - adapter nikdy nebol pripojeny k serveru
    X = 2 - spojenie s adapterom bolo prerusene
    X = 3 - chyba pri posielani dat na adapter
    X = 4 - odpoved na PING nedorazila (timeout alebo ina chyba)
    X = 5 - sprava nie je podporovana verziou protocolu na adapteri
*/
    pugi::xml_document doc;
    
    pugi::xml_node request_node = doc.append_child();
    request_node.set_name("request");
    request_node.append_attribute("type") = "ping";
    
    pugi::xml_node adapter_node = request_node.append_child();
    adapter_node.set_name("adapter");
    adapter_node.append_attribute("id") = gateway_id;
     
    std::ostringstream stream;
    doc.save(stream);
    
    doc.print(std::cout);
    
    m_request = stream.str();
    
    connect();
    
    
    std::cout << m_response << std::endl;
    
    return 1;
}

void GatewayInterface::sendSetState(long long gateway_id, long device_euid, int module_id, std::string new_value)
{
    std::cout << "GatewayInterface::sendSetState" << std::endl;
    /*
     <request type="switch">
        <sensor id="1111" type="0" onAdapter="12345">
            <value>ON</value>
        </sensor>
    </request>
     
     */
    
    pugi::xml_document doc;
    
    doc.print(std::cout); 
}

void GatewayInterface::connect()
{
    std::cout << "GatewayInterface::connect" << std::endl;
   
    asio::ip::tcp::resolver::query query("127.0.0.1", std::to_string(m_port).c_str());
    asio::ip::tcp::resolver resolver(m_io_service);
    asio::ip::tcp::resolver::iterator destination = resolver.resolve(query);
    asio::ip::tcp::endpoint endpoint;

    while (destination != asio::ip::tcp::resolver::iterator()) {
        
        endpoint = *destination++;
    }
    
    
    
    //asio::ip::tcp::resolver resolver(m_io_service);
    //asio::ip::tcp::endpoint endpoint = resolver.resolve("127.0.0.1", m_port);
    
    //asio::error_code error;
    //m_socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), m_port), error);
    //asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), m_port);
    m_socket.async_connect(endpoint,
      boost::bind(&GatewayInterface::handleConnect, this,
      asio::placeholders::error));
            
     /*       
    asio::ip::tcp::endpoint endpoint = *(m_resolver.resolve({"127.0.0.1", m_port}));
    m_socket.lowest_layer().async_connect(endpoint,
      boost::bind(&GatewayInterface::handleConnect, this,
      asio::placeholders::error));
    
      */ /*
    asio::ip::tcp::endpoint endpoint = *(m_resolver.resolve({"127.0.0.1", m_port}));
    m_socket.lowest_layer().async_connect(endpoint,
      boost::bind(&GatewayInterface::handleConnect, this,
      asio::placeholders::error));

     */
}

void GatewayInterface::handleConnect(const asio::error_code& error)
{
    std::cout << "GatewayInterface::handleConnect" << std::endl;
    
    if (!error) {
     std::cerr << "Connect failed: " << error.message() << std::endl;
       
      //std::cout << "Connect failed: " << error.message() << "\n";
      
    }
    else {
        std::cout << "connected" << std::endl;
        send();
    }
}

void GatewayInterface::send()
{
    std::cout << "GatewayInterface::send" << std::endl;
    //asio::error_code error;
    //asio::write(m_socket, asio::buffer(message), error);

    asio::async_write(m_socket,
      asio::buffer(m_request, m_request.length()),
      boost::bind(&GatewayInterface::handleWrite, this,
      asio::placeholders::error,
      asio::placeholders::bytes_transferred));
    
    /*
    if (error) {
        throw std::runtime_error("GatewayInterface could not send a message.");
    }
    */
    /*
    //size_t request_length = request.length();
    
     */
}


void GatewayInterface::handleWrite(const asio::error_code& error,
      size_t bytes_transferred)
{
    std::cout << "GatewayInterface::handleWrite" << std::endl;
    
    if (error) {
       std::cerr << "Write failed " << error.message() << std::endl;
    }
    else {
      //std::cout << "Write failed: " << error.message() << "\n";
      
     std::cout << "send successful" << std::endl;
        receive();
    }
  }


void GatewayInterface::receive()
{
    std::cout << "GatewayInterface::receive" << std::endl;
    asio::async_read(m_socket, m_reply_buffer,
                boost::bind(&GatewayInterface::handleRead, this,
                asio::placeholders::error, asio::placeholders::bytes_transferred));
    
    
    //size_t bytes_transferred = asio::read(m_socket, reply_buffer, asio::transfer_all(), error);
    
   
    /*
    asio::async_read(m_socket, m_reply_buffer,
                boost::bind(&GatewayInterface::handleRead, this,
                asio::placeholders::error, asio::placeholders::bytes_transferred));
     */
}

void GatewayInterface::handleRead(const asio::error_code& error,
      size_t bytes_transferred)
{
    std::cout << "GatewayInterface::handleRead" << std::endl;

    if (error && error != asio::error::eof) {
    
        throw std::runtime_error("GatewayInterface could not receive a message.");
    }
    else {
        std::cout << "receive succesfull" << std::endl;
        
   asio::streambuf::const_buffers_type bufstream = m_reply_buffer.data();
    
    std::string response(asio::buffers_begin(bufstream), asio::buffers_begin(bufstream) + bytes_transferred);
    // Vyčištění bufferu.
    m_reply_buffer.consume(bytes_transferred);
    
    m_response = response;
    
    std::cout << "handleRead - response: " << response << std::endl;
    }
}