/* 
 * File:   MsgInGetViews.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 17:03
 */

#include "MsgInGetViews.h"
#include "DBConnector.h"


const std::string MsgInGetViews::state = "getviews";

MsgInGetViews::MsgInGetViews(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
    
}


MsgInGetViews::~MsgInGetViews() {
}

int MsgInGetViews::getMsgAuthorization() {
    return GUEST;
}

string MsgInGetViews::createResponseMsgOut()
{  
    return envelopeResponse(R_VIEWS, DBConnector::getInstance().viewsList(_parredUserMail));
}
