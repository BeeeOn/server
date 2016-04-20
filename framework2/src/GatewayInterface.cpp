/* 
 * File:   GatewayInterface.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 13. April 2016
 */

#include "GatewayInterface.h"

#include <iostream>
#include <boost/bind.hpp>
#include <memory>
#include <sstream>
#include <string>
#include <stdexcept>

#include "pugixml.hpp"


GatewayInterface::GatewayInterface():
    m_io_service(),
    m_socket(m_io_service)
{
    connect();
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
    
    //std::strirequest = 
    
    send(stream.str());
    
    //receive();
    
    //std::cout << m_response << std::endl;
    
    //return 1;
}

void GatewayInterface::sendSetState(long long gateway_id, long device_euid, int module_id, int new_value)
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
    
    pugi::xml_node request_node = doc.append_child();
    request_node.set_name("request");
    request_node.append_attribute("type") = "switch";
    
    pugi::xml_node adapter_node = request_node.append_child();
    adapter_node.set_name("sensor");
    adapter_node.append_attribute("id") = static_cast<int>(device_euid);
    // This attribute in protocol is outdated. It is used to describe device_euid.
    adapter_node.append_attribute("type") = module_id;
    adapter_node.append_attribute("onAdapter") = gateway_id;
    
    std::ostringstream stream;
    doc.save(stream);
    
    doc.print(std::cout);
    
    send(stream.str());
}

void GatewayInterface::connect()
{
    // Connect to localhost and port of ada_server_sender.
    m_socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 7081));
}

void GatewayInterface::send(std::string request)
{
    std::cout << "GatewayInterface::send - enter" << std::endl;
    
    asio::streambuf request_streambuf;
    std::ostream request_stream(&request_streambuf);
    request_stream << request;

    // Send the message.
    asio::write(m_socket, request_streambuf);
    
    std::cout << "GatewayInterface::send - leave" << std::endl;
}

void GatewayInterface::receive()
{
    std::cout << "GatewayInterface::receive - enter" << std::endl;
   
    asio::streambuf response;
    asio::read_until(m_socket, response, "</reply>");

    std::iostream response_stream(&response);

    std::string content{ std::istreambuf_iterator<char>(response_stream),
                     std::istreambuf_iterator<char>() };
    
    m_response = content;
    
    std::cout << "GatewayInterface::receive - leave" << std::endl;
}
