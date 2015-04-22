/* 
 * File:   MsgInDelAdapter.cpp
 * Author: Pavel
 * 
 * Created on 7. dubna 2015, 9:09
 */

#include "MsgInDelAdapter.h"

const string MsgInDelAdapter::state = "deladapter";

MsgInDelAdapter::MsgInDelAdapter(char* msg, pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(msg, doc) {
}

MsgInDelAdapter::~MsgInDelAdapter() {
}

int MsgInDelAdapter::getMsgAuthorization() {
    return GUEST;
}

string MsgInDelAdapter::createResponseMsgOut() {
    
    return "";
}


