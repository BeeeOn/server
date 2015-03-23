/* 
 * File:   MsgInAlgorithmsRedirect.cpp
 * Author: pavel
 * 
 * Created on 16. březen 2015, 14:50
 */

#include "MsgInAlgorithmsRedirect.h"
#include "Config.h"
#include "SocketClient.h"


const std::string MsgInAlgorithmsRedirect::state = "algo";

MsgInAlgorithmsRedirect::MsgInAlgorithmsRedirect(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc)  {
}

MsgInAlgorithmsRedirect::~MsgInAlgorithmsRedirect() {
}

string MsgInAlgorithmsRedirect::createResponseMsgOut() {

    string r;
        try{
        SocketClient sc(Config::getInstance().getAlgorithmPort());    
                        
        Logger::getInstance(Logger::DEBUG3)<<"Algo communication"<<endl; 
        sc.write(_msg);
        r = sc.read();
    }catch(...){
        throw ServerException(ServerException::UI2ALGO);
    }
    Logger::getInstance(Logger::DEBUG3)<<"S2S communication: "<< r<<endl; 
        
    return r;
}

int MsgInAlgorithmsRedirect::getMsgAuthorization() {
    return GUEST;
}

