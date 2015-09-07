/* 
 * File:   MsgInDelAdapter.cpp
 * Author: Pavel
 * 
 * Created on 7. dubna 2015, 9:09
 */

#include "GateDelete.h"
#include "../DAO/DAOAdapters.h"
#include "../DAO/DAOUsers.h"
#include "SocketClient.h"
#include "Config.h"

using namespace std;

const string GateDelete::state = "deletegate";

GateDelete::GateDelete(pugi::xml_document* doc): IMsgInLoginRequired(doc) {
}

GateDelete::~GateDelete() {
}

int GateDelete::getMsgAuthorization() {
    return permissions::guest;
}
//Pokud je uživatel SU a tohle udělá, smaže se všechno, musím poslat ada_server zprávu 
string GateDelete::createResponseMsgOut() {
    
    int gateId = _doc->child(proto::communicationNode).child(proto::adapterNode).attribute(proto::gatewayIdAttr).as_int(-1);
    
    string role = DAOUsers::getInstance().getUserRoleM(_userId, gateId);
    
    if(role != proto::roleSuperuserAttr)
    {
        if (DAOAdapters::getInstance().delUsersAdapter(_gatewayId, _userId) > 0)
        {
            return getXMLreply(proto::replyTrue);
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
            return getNegativeXMLReply(ServerException::SERVER2SERVER);
        }
        Logger::getInstance(Logger::DEBUG3)<<"S2S communication: "<< r<<endl; 

        if(r == "<reply>true</reply>")
            return getXMLreply(proto::replyTrue);
        else
             return getNegativeXMLReply(ServerException::SERVER2SERVER);
    }
    return getNegativeXMLReply(ServerException::ROLE);
    
}


