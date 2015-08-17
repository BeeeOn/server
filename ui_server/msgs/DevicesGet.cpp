#include "DevicesGet.h"

using namespace std;

const string DevicesGet::state = "getdevs";

DevicesGet::DevicesGet(pugi::xml_document* doc): IMsgInLoginRequired(doc)
{
}

DevicesGet::~DevicesGet(void)
{
}

int DevicesGet::getMsgAuthorization() {
    return GUEST;
}


string DevicesGet::createResponseMsgOut()
{
    long long gateId;
    int mac;
    
    vector<long long> gatesVec;
    vector<int> devicesVec;
    
    pugi::xml_node gateNode =  _doc->child(P_COMMUNICATION).child(P_ADAPTER);
    pugi::xml_node deviceNode;
    
    for (; gateNode; gateNode = gateNode.next_sibling(P_ADAPTER))
    {
        gateId = gateNode.attribute(P_IN_ADAPTER_ID).as_llong();
        gatesVec.push_back(gateId);
        
        deviceNode =  _doc->child(P_COMMUNICATION).child(P_ADAPTER).child(P_FACILITY);
        
        for (; deviceNode; deviceNode = deviceNode.next_sibling(P_FACILITY))
        {
            mac = deviceNode.attribute(P_IN_FACILITY_ID).as_int();
            devicesVec.push_back(mac);            
        }
    }
    
//    vector<device> devicesVec;
//    vector<string> adaptersVec;
//    vector<long long> gateVec;
//    vector<int> deviceVec;
//    
//    pugi::xml_node adapterNode =  _doc->child(P_COMMUNICATION).child(P_ADAPTER);
//    pugi::xml_node facilityNode;
//    pugi::xml_node deviceNode;
//    
//    for (; adapterNode; adapterNode = adapterNode.next_sibling(P_ADAPTER))
//    {
//        adapterId = adapterNode.attribute(P_IN_ADAPTER_ID).value();
//        adaptersVec.insert(adaptersVec.begin(), adapterId);
//        
//        facilityNode =  _doc->child(P_COMMUNICATION).child(P_ADAPTER).child(P_FACILITY);
//        
//        for (; facilityNode; facilityNode = facilityNode.next_sibling(P_FACILITY))
//        {
//            facilityMac = facilityNode.attribute(P_IN_FACILITY_ID).value();
//            //facilitiesVec.insert(facilitiesVec.begin(), facilityMac);
//            
//            deviceNode =  facilityNode.child(P_DEVICE);
//            for (; deviceNode; deviceNode = deviceNode.next_sibling(P_DEVICE))
//            {
//                deviceType = deviceNode.attribute(P_IN_DEVICE_TYPE).value();
//                //devicesVec.insert(devicesVec.begin(), deviceType);
//                
//                device d;
//                d.id = facilityMac;
//                d.type = deviceType;
//                if(d.id == "" || d.type == "")
//                    continue;
//                //cout<<d.id<<d.type<<endl;
//                devicesVec.insert(devicesVec.begin(), d);
//            }
//        }
//    }
    
    return envelopeResponse(R_DEVICES, DAODevices::getInstance().getXMLdevices(_userId, gatesVec, devicesVec));
}