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

const std::string GateAdd::state = "addadapter";

GateAdd::GateAdd(pugi::xml_document* doc): IMsgInLoginRequired(doc) {
}

GateAdd::~GateAdd() {
}

string GateAdd::createResponseMsgOut() {
    pugi::xml_node adapterNode =  _doc->child(P_COMMUNICATION);
    string adapterIDStr = adapterNode.attribute(P_ADAPTER_ID).as_string();
    long long int adapterId = stoll(adapterIDStr);// TODO can throw exc: invalid argument
    
    string adapterName = adapterNode.attribute(P_ADAPTER_NAME).value();
    
    if(DAOAdapters::getInstance().parAdapterWithUserIfPossible(adapterId, adapterName, _userId) == 0){
        if(DAOAdapters::getInstance().isAdapterInDB(adapterId) == 0)
            throw ServerException(ServerException::ADAPTER_ID);
        else
            throw ServerException(ServerException::ADAPTER_TAKEN);
    }
    
    
        string r;
    try{
        SocketClient sc(Config
        ::getInstance().getGamificationPort());    
                        
        Logger::getInstance(Logger::DEBUG3)<<"Gami communication"<<endl; 
        
        pugi::xml_node node = _doc->child(P_COMMUNICATION);
        node.append_attribute("userid")=_userId;
        node.append_attribute("id")="61";
        
        string algoStr = node_to_string(node);
        Logger::debug3() << "newadapter _ toGami: " << algoStr << endl;
        sc.write(algoStr.c_str());
    }catch(...){
        Logger::debug3() << "newadapter _ toGami: FAIL" << endl;
    }
    
    return genOutputXMLwithVersionAndState(R_TRUE);
}

int GateAdd::getMsgAuthorization() {
    return GUEST;
}

