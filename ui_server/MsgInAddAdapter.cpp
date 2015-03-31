/* 
 * File:   MsgInAddAdapter.cpp
 * Author: pavel
 * 
 * Created on 3. září 2014, 13:11
 */

#include "MsgInAddAdapter.h"

const std::string MsgInAddAdapter::state = "addadapter";

MsgInAddAdapter::MsgInAddAdapter(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}

MsgInAddAdapter::~MsgInAddAdapter() {
}

string MsgInAddAdapter::createResponseMsgOut() {
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
    
    return envelopeResponse(R_TRUE);
}

int MsgInAddAdapter::getMsgAuthorization() {
    return GUEST;
}

