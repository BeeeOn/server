/* 
 * File:   MsgInPartial.cpp
 * Author: pavel
 * 
 * Created on 3. červen 2014, 8:03
 */

#include "MsgInSetDevices.h"
#include "ServerException.h"
#include "../DAO/DAODevices.h"
using namespace std;
const string MsgInDevices::state = "setdevs";

MsgInDevices::MsgInDevices(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc){
}


MsgInDevices::~MsgInDevices() {
}

int MsgInDevices::getMsgAuthorization() {
    return ADMIN;
}


string MsgInDevices::createResponseMsgOut()
{    
    bool error = false;
    string errorText;
    
    
    pugi::xml_node facilityNode =  _doc->child(P_COMMUNICATION).child(P_FACILITY);
    pugi::xml_node deviceNode;
    
    for (; facilityNode; facilityNode = facilityNode.next_sibling(P_FACILITY)){
        
        string id = facilityNode.attribute(P_FACILITY_ID).value();
        string init = facilityNode.attribute(P_FACILITY_INIT).value();
        string locationId = facilityNode.attribute(P_FACILITY_LOCATION_ID).value();
        string refresh = facilityNode.attribute(P_FACILITY_REFRESH).value();
        
        if(init !="" || locationId!="" || refresh!="")
            DAODevices::getInstance().updateFacility(_adapterId, id, init, locationId, refresh);
        
        deviceNode =  facilityNode.child(P_DEVICE);
                
        for (; deviceNode; deviceNode = deviceNode.next_sibling(P_DEVICE)){
            
            string type = deviceNode.attribute(P_IN_DEVICE_TYPE).value();
            string visibility = deviceNode.attribute(P_DEVICE_VISIBILITY).value();
            string name = deviceNode.attribute(P_DEVICE_NAME).value();
            
            if(visibility !="" || name !="")
                if( DAODevices::getInstance().updateDevice(_adapterId, id, type, name, visibility) == 0){
                    error = true;
                    errorText += "<device id=\"" + id + "\" type=\"" + type + "\" />";
                }
        }
            
    }
        
    
    if(error)   
        throw ServerException(ServerException::DEVICE_ID_TYPE,errorText);
    
    return envelopeResponse(R_TRUE);
}