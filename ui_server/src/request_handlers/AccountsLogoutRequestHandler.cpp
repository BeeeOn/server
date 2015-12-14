#include "AccountsLogoutRequestHandler.h"
#include "../DAO/DAOUsers.h"
#include "../lib/pugixml.hpp"
#include "TokenChecker.h"
#include "SessionsTable.h"

using namespace std;

const std::string AccountsLogoutRequestHandler::state = "logout";

AccountsLogoutRequestHandler::AccountsLogoutRequestHandler(shared_ptr<pugi::xml_document> doc, DAOMobileDevices & daoMobileDevices, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginRequired(doc, sessionTable), 
        _daoMobileDevices(daoMobileDevices)
{
}

AccountsLogoutRequestHandler::~AccountsLogoutRequestHandler() 
{
}

int AccountsLogoutRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

string AccountsLogoutRequestHandler::createResponseMsgOut() 
{
    _daoMobileDevices.deletepushNotification(_userId, "");
    _sessionTable.removeEntry(_token);

    return getXMLreply(proto::replyTrue);
}




