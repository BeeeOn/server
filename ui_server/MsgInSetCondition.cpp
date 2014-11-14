/* 
 * File:   MsgInSetCondition.cpp
 * Author: pavel
 * 
 * Created on 16. září 2014, 9:27
 */

#include "MsgInSetCondition.h"
#include "DBConnector.h"

const string MsgInSetCondition::state = "setcondition";

MsgInSetCondition::MsgInSetCondition(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}

MsgInSetCondition::~MsgInSetCondition() {
}

int MsgInSetCondition::getMsgAuthorization() {
    return USER;
}

string MsgInSetCondition::createResponseMsgOut() {
    string condName = _doc->child("communication").child("condition").attribute("name").value();
    string condType = _doc->child("communication").child("condition").attribute("type").value();
    
    ostringstream oss;
    pugi::xml_node node = _doc->child("communication").child("condition");
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
        it->print(oss,"",pugi::format_raw);
    string condXml = oss.str();
    //TODO kontrola validity podminky?
    //TODO kontrola typu + test
    string condId = DBConnector::getInstance().insertNewCondition(_gUserId, condName, condType, condXml);
    
    
    //return new MsgOutConditionCreated(_comId, condId, _state);
    return envelopeResponse(R_TRUE);
}


