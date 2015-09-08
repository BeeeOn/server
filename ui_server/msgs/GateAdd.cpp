/* 
 * File:   MsgInAddAdapter.cpp
 * Author: pavel
 * 
 * Created on 3. září 2014, 13:11
 */

#include "GateAdd.h"
#include "SocketClient.h"
#include "save_custom_writer.h"
#include "Config.h"

using namespace std;

const std::string GateAdd::state = "addgate";

GateAdd::GateAdd(pugi::xml_document* doc): IMsgInLoginRequired(doc) {
}

GateAdd::~GateAdd() {
}

string GateAdd::createResponseMsgOut() {
    pugi::xml_node adapterNode =  _doc->child(proto::communicationNode).child(proto::adapterNode);
    
    long long int adapterId = adapterNode.attribute(proto::gatewayIdAttr).as_llong();
    string adapterName = adapterNode.attribute(proto::gatewayNameAttr).value();
    
    if(DAOUsersAdapters::getInstance().parAdapterWithUserIfPossible(adapterId, adapterName, _userId) == 0)
    {
        if(DAOAdapters::getInstance().isAdapterInDB(adapterId) == 0)
        {
            return getNegativeXMLReply(ServerException::ADAPTER_ID);
        }
        else
        {
            if(DAOUsers::getInstance().isAdapterParred(_userId, adapterId))
                return getNegativeXMLReply(ServerException::ADAPTER_ACCESSIBLE);;
            return getNegativeXMLReply(ServerException::ADAPTER_TAKEN);
        }
    }
    
    
    try{
        SocketClient sc(Config::getInstance().getGamificationPort());    
                        
        Logger::getInstance(Logger::DEBUG3)<<"Gami communication"<<endl; 
        
        pugi::xml_node node = _doc->child(proto::communicationNode);
        node.append_attribute("userid")=_userId;
        node.append_attribute("id")="61";//Magic for gamification
        
        string algoStr = node_to_string(node);
        Logger::debug3() << "newadapter _ toGami: " << algoStr << endl;
        sc.write(algoStr.c_str());
    }catch(...){
        Logger::debug3() << "newadapter _ toGami: FAIL" << endl;
    }
    
    return getXMLreply(proto::replyTrue);
}

int GateAdd::getMsgAuthorization() {
    return permissions::guest;
}

