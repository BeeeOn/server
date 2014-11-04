/* 
 * File:   MsgInPartial.cpp
 * Author: pavel
 * 
 * Created on 3. červen 2014, 8:03
 */

#include "MsgInPartial.h"
#include "ServerException.h"
using namespace std;
const string MsgInPartial::state = "partial";

MsgInPartial::MsgInPartial(char* msg, pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(msg, doc){
}


MsgInPartial::~MsgInPartial() {
}

IMsgOut* MsgInPartial::createResponseMsgOut()
{
    if( !isComIdValid())
        return new MsgOutReSign();
    
    bool error = false;
    string errorText;
    pugi::xml_node deviceNode =  _doc->child("communication").child("device");

    for (; deviceNode; deviceNode = deviceNode.next_sibling("device"))
    {
        string id = deviceNode.attribute("id").value();
        string type = deviceNode.attribute("type").value();
        
        string init = deviceNode.attribute("initialized").value();
        string visibility = deviceNode.attribute("visibility").value();

        string locationId = deviceNode.child("location").attribute("id").value();

        string name = deviceNode.child("name").child_value();
        string refresh = deviceNode.child("refresh").child_value();
        //Value and log_en is not longer supported here
        string value;// = deviceNode.child("value").child_value();
         
         string log_en;// = deviceNode.child("logging").attribute("enabled").value();
         Logger::getInstance(Logger::DEBUG)<<"update senzor: " <<id<<"....";
         if( DBConnector::getInstance().updateDevice(id, type, name, init, locationId, refresh, value, log_en, visibility) == false){
             error = true;
             errorText += "<device id=\"" + id + "\" type=\"" + type + "\" />";
         }
         
         Logger::getInstance(Logger::DEBUG)<<"OK\n";
    }
    
    if(error)   
        throw ServerException(ServerException::DEVICE_ID_TYPE,errorText);
    
    return new MsgOutTrue(_comId, _state);
}