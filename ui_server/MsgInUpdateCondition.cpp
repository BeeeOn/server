/* 
 * File:   MsgInUpdateCondition.cpp
 * Author: pavel
 * 
 * Created on 16. září 2014, 15:47
 */

#include "MsgInUpdateCondition.h"
#include "DBConnector.h"

const string MsgInUpdateCondition::state = "updatecondition";

MsgInUpdateCondition::MsgInUpdateCondition(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}


MsgInUpdateCondition::~MsgInUpdateCondition() {
}

int MsgInUpdateCondition::getMsgAuthorization() {
    return USER;
}

string MsgInUpdateCondition::createResponseMsgOut() {
    pugi::xml_node node = _doc->child("communication").child("condition");
    
    string condId = node.attribute("id").value();
    string condName = node.attribute("name").value();
    string condType = node.attribute("type").value();
    
    
    ostringstream oss;
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
        it->print(oss,"",pugi::format_raw);
    
    string condXml = oss.str();
    
    //TODO testovat validitu xml a propojit s users
    if(DBConnector::getInstance().updateCondition(condId, condName, condType, condXml) ==0)
        throw ServerException(ServerException::WRONG_COND_OR_EMTPY_COND);
    
//    return new MsgOutTrue(_comId, _state);
    return envelopeResponse(R_TRUE);
}



