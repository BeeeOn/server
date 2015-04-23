/* 
 * File:   MsgInAdapterListen.cpp
 * Author: pavel
 * 
 * Created on 22. červenec 2014, 10:14
 */


#include "MsgInAdapterListen.h"
#include "Config.h"

using namespace std;
const std::string MsgInAdapterListen::state = "scanmode";

MsgInAdapterListen::MsgInAdapterListen(char* msg, pugi::xml_document* doc) : IMsgInLoginAndAdapterAccessRequired(msg, doc) {
}

MsgInAdapterListen::~MsgInAdapterListen() {
}

int MsgInAdapterListen::getMsgAuthorization() {
    return SUPERUSER;
}

string MsgInAdapterListen::createResponseMsgOut()
{       
    string r ;
    try{
        SocketClient sc(Config::getInstance().getActivityPort());    

        sc.write("<request type=\"listen\">"
                            "<adapter id=\""+_adapterId+"\"/>"
                        "</request>");
        r= sc.read();
    }catch(...){
        throw ServerException(ServerException::SERVER2SERVER);
    }
    
    Logger::getInstance(Logger::DEBUG3)<<"S2S communication: "<< r<<endl; 
    
    if(r == "<reply>true</reply>"){
        return envelopeResponse(R_TRUE);
    }else{
        throw ServerException(ServerException::ADAPTER_LISTEN_FAIL);
    }
}
