#include "DevicesGet.h"

using namespace std;

const string DevicesGet::state = "getdevices";

DevicesGet::DevicesGet(pugi::xml_document* doc): IMsgInLoginRequired(doc)
{
}

DevicesGet::~DevicesGet(void)
{
}

int DevicesGet::getMsgAuthorization() {
    return permissions::guest;
}


string DevicesGet::createResponseMsgOut()
{
    long long gateId;
    int mac;
    
    vector<long long> gatesVec;
    vector<int> devicesVec;
    
    pugi::xml_node gateNode =  _doc->child(proto::communicationNode).child(proto::adapterNode);
    pugi::xml_node deviceNode;
    
    for (; gateNode; gateNode = gateNode.next_sibling(proto::adapterNode))
    {
        gateId = gateNode.attribute(proto::gatewayIdAttr).as_llong();
        gatesVec.push_back(gateId);
        
        deviceNode =  _doc->child(proto::communicationNode).child(proto::adapterNode).child(proto::deviceNode);
        
        for (; deviceNode; deviceNode = deviceNode.next_sibling(proto::deviceNode))
        {
            mac = deviceNode.attribute(proto::deviceIdAttr).as_int();
            devicesVec.push_back(mac);            
        }
    }
    
    
    return envelopeResponse(proto::replyDevices, DAODevices::getInstance().getXMLdevices(_userId, gatesVec, devicesVec));
}