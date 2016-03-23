#include "AdaServerCommunicator.h"
#include "DAO/DAO.h"
#include "SocketClient.h"
#include "ServerException.h"
#include "SecuredAsioServlet.h"
#include <string>

#include "../lib/pugixml.hpp"

using namespace std;

AdaServerCommunicator::AdaServerCommunicator(int port) 
    :   _port(port) 
{
}

AdaServerCommunicator::~AdaServerCommunicator() 
{
}

int AdaServerCommunicator::sendSetState(gatewayId_64b gatewayId, deviceId_64b deviceId, int deviceType, string newValue) 
{
    string r;
    try
    {
        SocketClient sc(_port);    
        string request = "<request type=\"switch\">"
                            "<sensor id=\"" + to_string(deviceId) + "\" type=\"" + to_string(deviceType) + "\" onAdapter=\"" + to_string(gatewayId) + "\">"
                                    "<value>" + newValue + "</value>"
                            "</sensor>"
                        "</request>";
                        
        Logger::getInstance(Logger::DEBUG3) << "request to Ada: " << request << endl; 
        sc.write(request);
        r = sc.readUntilendTag("</reply>");
    }
    catch(exception& e)
    {
        Logger::debug() << "ada_server comm err: " << e.what() << endl;
        return ServerException::SERVER2SERVER;
    }
    Logger::getInstance(Logger::DEBUG3) << "response from Ada " << r << endl; 
    if(r == "")
        return ServerException::ADA_SERVER_TIMEOUT;
    else if(r == "<reply>true</reply>")
        return ServerException::OK;
    else
        return ServerException::GATEWAY_ACTION_FAIL;
}

int AdaServerCommunicator::sendUnregisterDevice(gatewayId_64b gatewayId, deviceId_64b deviceId) 
{
    string r ;
    try
    {
        SocketClient sc(_port);    

        sc.write("<request type=\"delete\">"
                            "<sensor id=\""+to_string(deviceId)+"\" onAdapter=\""+to_string(gatewayId)+"\" />"
                        "</request>");
        r = sc.readUntilendTag("</reply>");
    }
    catch(exception& e)
    {
        Logger::debug() << "ada_server comm err: " << e.what() << endl;
        return ServerException::SERVER2SERVER;
    }
    Logger::getInstance(Logger::DEBUG3) << "response from Ada " << r << endl; 
    
    if(r == "")
        return ServerException::ADA_SERVER_TIMEOUT;
    else if(r == "<reply>true</reply>")
        return ServerException::OK;
    else
        return ServerException::GATEWAY_ACTION_FAIL;  
}

int AdaServerCommunicator::sendGatewayStartListen(gatewayId_64b gatewayId) 
{
    string r;
    pugi::xml_document reply;
    try
    {
        SocketClient sc(_port);    

        string request = "<request type=\"listen\">"
                                "<adapter id=\"";
        request.append(to_string(gatewayId));
        request.append("\" />"
                            "</request>");
        
        sc.write(request);
        r = sc.read();
    }
    catch(exception& e)
    {
        Logger::debug() << "ada_server comm err: " << e.what() << endl;
        return ServerException::SERVER2SERVER;
    }
    
    Logger::getInstance(Logger::DEBUG3) << "response from Ada " << r << endl; 
    pugi::xml_parse_result res = reply.load_string(r.c_str());
    Logger::getInstance(Logger::DEBUG3) << "response from Ada in xml val: " << reply.child("reply") << endl; 
    
    if(r == "")
        return ServerException::ADA_SERVER_TIMEOUT;
    //else if(r == "<reply>true</reply>")
    else if(reply.child("reply"))
        return ServerException::OK;
    else
        return ServerException::GATEWAY_ACTION_FAIL;
}

int AdaServerCommunicator::sendGatewayStartSearch(gatewayId_64b gatewayId, std::string ip, std::string deviceEuid)
{
    string r;
    try
    {
        SocketClient sc(_port);    

        string request = "<request type=\"search\">"
                                "<adapter id=\"" + to_string(gatewayId) + "\" "
                                        "deviceip=\"" + ip + "\" "
                                        "deviceeuid=\"" + deviceEuid + "\" "
                
                                "/>"
                            "</request>";
        
        sc.write(request);
        r = sc.read();
    }
    catch(exception& e)
    {
        Logger::debug() << "ada_server comm err: " << e.what() << endl;
        return ServerException::SERVER2SERVER;
    }
    
    Logger::getInstance(Logger::DEBUG3) << "response from Ada " << r << endl; 
    
    if(r == "")
        return ServerException::ADA_SERVER_TIMEOUT;
    else if(r == "<reply>true</reply>")
        return ServerException::OK;
    else
        return ServerException::GATEWAY_ACTION_FAIL;
}

    
