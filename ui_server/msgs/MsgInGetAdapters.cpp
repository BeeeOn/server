#include "MsgInGetAdapters.h"

using namespace std;

const std::string MsgInGetAdapters::state = "getadapters";

MsgInGetAdapters::MsgInGetAdapters(pugi::xml_document* doc): IMsgInLoginRequired(doc)
{
}

MsgInGetAdapters::~MsgInGetAdapters(void)
{
}
int MsgInGetAdapters::getMsgAuthorization() {
    return GUEST;
}

string MsgInGetAdapters::createResponseMsgOut()
{
    return envelopeResponse(R_ADAPTERS, DAOUsers::getInstance().getXMLusersAdapters(_userId));
}