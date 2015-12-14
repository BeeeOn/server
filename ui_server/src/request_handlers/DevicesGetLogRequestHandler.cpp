#include "DevicesGetLogRequestHandler.h"
#include "../lib/pugixml.hpp"
#include "../DAO/DAODevices.h" 
#include "../DAO/DAOlogs.h"

#include <climits>
using namespace std;

const string DevicesGetLogRequestHandler::state = "getlog";

DevicesGetLogRequestHandler::DevicesGetLogRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, DAOlogs & daoLogs, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginAndGateAccessRequired(doc, daoUsers, sessionTable),
        _daoLogs(daoLogs)
{
}

DevicesGetLogRequestHandler::~DevicesGetLogRequestHandler() 
{
}

int DevicesGetLogRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

string timestampToHReadble(const time_t rawtime)
{
    struct tm * dt;
    char buffer [30];
    dt = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%c", dt);
    return string(buffer);
}


string DevicesGetLogRequestHandler::createResponseMsgOut()
{                        
    pugi::xml_node logsNode =  _doc->child(proto::requestNode).child(proto::logsNode);

    long long logFrom =  logsNode.attribute(proto::logsFromAttr).as_llong(0);
    long long logTo = logsNode.attribute(proto::logsToAttr).as_llong(LLONG_MAX);
    string type =  logsNode.attribute(proto::logsFunctionAttr).value();
    long long interval = logsNode.attribute(proto::logsIntervalAttr).as_llong(0);
    
    device d;
    d.id =  logsNode.attribute(proto::logsDeviceIdAttr).as_ullong(0);
    d.type =  logsNode.attribute(proto::logsModuleIdAttr).as_int(-1);
    
    Logger::debug() << "get logs from " << timestampToHReadble(logFrom) << "to " << timestampToHReadble(logTo) << endl;
    
    return getDataXMLreply(proto::replyLogs, _daoLogs.getXMLDeviceLog(_gatewayId, d, logFrom, logTo, type , interval));
}