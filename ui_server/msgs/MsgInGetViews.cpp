/* 
 * File:   MsgInGetViews.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 17:03
 */

#include "MsgInGetViews.h"
#include "DBConnector.h"

using namespace std;

const std::string MsgInGetViews::state = "getviews";

MsgInGetViews::MsgInGetViews(pugi::xml_document* doc): IMsgInLoginRequired(doc) {
    
}


MsgInGetViews::~MsgInGetViews() {
}

int MsgInGetViews::getMsgAuthorization() {
    return GUEST;
}

string MsgInGetViews::createResponseMsgOut()
{  
    //return envelopeResponse(R_VIEWS, DBConnector::getInstance().viewsList(_userId));
    return "";
}
