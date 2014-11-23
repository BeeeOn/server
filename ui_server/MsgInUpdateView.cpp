/* 
 * File:   MsgInUpdateView.cpp
 * Author: pavel
 * 
 * Created on 2. červenec 2014, 16:35
 */

#include "MsgInUpdateView.h"


using namespace std;

const std::string MsgInUpdateView::state = "updateview";

MsgInUpdateView::MsgInUpdateView(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {

}


MsgInUpdateView::~MsgInUpdateView() {
}

int MsgInUpdateView::getMsgAuthorization() {
    return GUEST;
}

string MsgInUpdateView::createResponseMsgOut()
{    
    pugi::xml_node deviceNode =  _doc->child("communication").child("device");
    string deviceId;
    string deviceType;
    string action;
    string viewName = _doc->child("communication").attribute("name").value();
    string viewIcon = _doc->child("communication").attribute("icon").value();
    int vi;
    try{
      vi = stoi(viewIcon);
    }catch(...){
        throw ServerException(ServerException::ICON);
    }
    if(vi<0)//TODO horni omezeni
        throw ServerException(ServerException::ICON);
    //TODO castecna uprava 
    
    for (; deviceNode; deviceNode = deviceNode.next_sibling("device"))
    {
        deviceId = deviceNode.attribute("id").value();
        deviceType = deviceNode.attribute("type").value();
        action = deviceNode.attribute("action").value();
        
        if(action.compare("add") == 0)
            DBConnector::getInstance().addDeviceToView(viewName,_gUserId, deviceId, deviceType);
        else if(action.compare("remove") == 0)
            DBConnector::getInstance().removeDeviceFromView(viewName,_gUserId, deviceId, deviceType);
        else
            throw ServerException(ServerException::ACTION);
    }
    
   return envelopeResponse(R_TRUE);
}
