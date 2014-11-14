/* 
 * File:   MsgInSetAction.cpp
 * Author: pavel
 * 
 * Created on 17. září 2014, 12:37
 */

#include "MsgInSetAction.h"
#include "DBConnector.h"

const string MsgInSetAction::state="setaction";


MsgInSetAction::MsgInSetAction(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc)  {
}

MsgInSetAction::~MsgInSetAction() {
}

int MsgInSetAction::getMsgAuthorization() {
    return USER;
}

string MsgInSetAction::createResponseMsgOut() {
    
    string actionName = _doc->child("communication").child("complexaction").attribute("name").value();
    
    ostringstream oss;
    pugi::xml_node node = _doc->child("communication").child("complexaction");
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
        it->print(oss,"",pugi::format_raw);
    string actionXml = oss.str();
    //TODO kontrola validity akce?
    string newActionId = DBConnector::getInstance().insertNewAction(_gUserId, actionName, actionXml);
    //return new MsgOutActionCreated(_comId, newActionId, _state);
    return envelopeResponse(R_TRUE);
}


