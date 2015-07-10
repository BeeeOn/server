#include "GatesGetConnected.h"

using namespace std;

const std::string GatesGetConnected::state = "getadapters";

GatesGetConnected::GatesGetConnected(pugi::xml_document* doc): IMsgInLoginRequired(doc)
{
}

GatesGetConnected::~GatesGetConnected(void)
{
}
int GatesGetConnected::getMsgAuthorization() {
    return GUEST;
}

string GatesGetConnected::createResponseMsgOut()
{
    return envelopeResponse(R_ADAPTERS, DAOUsers::getInstance().getXMLusersAdapters(_userId));
}