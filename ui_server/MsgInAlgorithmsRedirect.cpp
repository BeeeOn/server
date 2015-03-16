/* 
 * File:   MsgInAlgorithmsRedirect.cpp
 * Author: pavel
 * 
 * Created on 16. březen 2015, 14:50
 */

#include "MsgInAlgorithmsRedirect.h"
#include "Config.h"


const std::string MsgInAlgorithmsRedirect::state = "algo";

MsgInAlgorithmsRedirect::MsgInAlgorithmsRedirect(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc)  {
}

MsgInAlgorithmsRedirect::~MsgInAlgorithmsRedirect() {
}

string MsgInAlgorithmsRedirect::createResponseMsgOut() {
    //redirect to algo...
    Config::getInstance().getAlgorithmPort();
    return "algook";
}

int MsgInAlgorithmsRedirect::getMsgAuthorization() {
    return GUEST;
}

