#include "UnknownRequestHandler.h"
#include "ServerException.h"

using namespace std;

const std::string UnknownRequestHandler::state = "";

UnknownRequestHandler::UnknownRequestHandler(shared_ptr<pugi::xml_document> doc): IRequestHandlerWithoutUac(doc){
}

UnknownRequestHandler::~UnknownRequestHandler() {
}

int UnknownRequestHandler::getMsgAuthorization() {
    return permissions::everyone;
}

string UnknownRequestHandler::createResponseMsgOut()
{
    return getNegativeXMLReply(ServerException::MSG_UNKNOWN, "not supported by server");
}