#include "GatesGetConnected.h"

using namespace std;

const std::string GatesGetConnected::state = "getgates";

GatesGetConnected::GatesGetConnected(pugi::xml_document* doc): IMsgInLoginRequired(doc)
{
}

GatesGetConnected::~GatesGetConnected(void)
{
}
int GatesGetConnected::getMsgAuthorization() {
    return permissions::guest;
}

string GatesGetConnected::createResponseMsgOut()
{
    return envelopeResponse(proto::replyGates, DAOUsers::getInstance().getXMLusersAdapters(_userId));
}