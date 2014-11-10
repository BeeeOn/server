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
    int adapterId = adapterNode.attribute(P_ADAPTER_ID).as_int(0);
    string adapterName = adapterNode.attribute(P_ADAPTER_NAME).value();
    
    if(DBConnector::getInstance().parAdapterWithUserIfPossible(adapterId, adapterName, _parredUserMail) == 0)
        throw ServerException(ServerException::ADAPTER_ID);
    
    return envelopeResponse(R_TRUE);
}

int MsgInAddAdapter::getMsgAuthorization() {
    return GUEST;
}
