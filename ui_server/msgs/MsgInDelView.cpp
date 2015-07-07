/* 
 * File:   MsgInDelView.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 16:50
 */

#include "MsgInDelView.h"


using namespace std;

const std::string MsgInDelView::state = "delview";

MsgInDelView::MsgInDelView(pugi::xml_document* doc): IMsgInLoginRequired(doc) {

}

MsgInDelView::~MsgInDelView() {
}

int MsgInDelView::getMsgAuthorization() {
    return GUEST;
}

string MsgInDelView::createResponseMsgOut()
{
    //string viewName = _doc->child("communication").attribute("name").value();
    //DBConnector::getInstance().delView(viewName,_userId);

    return envelopeResponse(R_TRUE);
}

