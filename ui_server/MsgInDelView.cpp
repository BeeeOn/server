/* 
 * File:   MsgInDelView.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 16:50
 */

#include "MsgInDelView.h"


using namespace std;

const std::string MsgInDelView::state = "delview";

MsgInDelView::MsgInDelView(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {

}

MsgInDelView::~MsgInDelView() {
}

int MsgInDelView::getMsgAuthorization() {
    return GUEST;
}

string MsgInDelView::createResponseMsgOut()
{
    string viewName = _doc->child("communication").attribute("name").value();
    DBConnector::getInstance().delView(viewName,_gUserId);

    return envelopeResponse(R_TRUE);
}

