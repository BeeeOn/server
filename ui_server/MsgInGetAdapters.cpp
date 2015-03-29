#include "MsgInGetAdapters.h"


const std::string MsgInGetAdapters::state = "getadapters";

MsgInGetAdapters::MsgInGetAdapters(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc)
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