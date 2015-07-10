/* 
 * File:   MsgInDelAdapter.cpp
 * Author: Pavel
 * 
 * Created on 7. dubna 2015, 9:09
 */

#include "GateDelete.h"
#include "../DAO/DAOAdapters.h"
#include "SocketClient.h"
#include "Config.h"

using namespace std;

const string GateDelete::state = "deladapter";

GateDelete::GateDelete(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}

GateDelete::~GateDelete() {
}

int GateDelete::getMsgAuthorization() {
    return GUEST;
}
//Pokud je uživatel SU a tohle udělá, smaže se všechno, musím poslat ada_server zprávu 
string GateDelete::createResponseMsgOut() {
    
    if(_role != "superuser")
    {
        if (DAOAdapters::getInstance().delUsersAdapter(_adapterId, _userId) > 0)
        {
            return genOutputXMLwithVersionAndState(R_TRUE);
        }
    }
    else
    {
        //delete whole gate with connected data
        
        string r ;
        try{
            SocketClient sc(Config::getInstance().getActivityPort());    
            string request = "<request type=\"delete\">"
                                "<adapter id=\""+_adapterId+"\" />"
                            "</request>";

            Logger::getInstance(Logger::DEBUG3)<<"S2S communication: "<< request<<endl; 
            sc.write(request);
            r = sc.readUntilendTag("</reply>");
        }catch(...){
            throw ServerException(ServerException::SERVER2SERVER);
        }
        Logger::getInstance(Logger::DEBUG3)<<"S2S communication: "<< r<<endl; 

        if(r == "<reply>true</reply>")
        return envelopeResponse(R_TRUE);
        else
             throw ServerException(ServerException::SERVER2SERVER);
    }
        
    _outputMainNode.append_attribute(P_ERRCODE) = ServerException::ROLE;
    return genOutputXMLwithVersionAndState(R_FALSE);
    
}


