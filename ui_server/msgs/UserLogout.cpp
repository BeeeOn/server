
#include "UserLogout.h"
#include "../DAO/DAOUsers.h"
#include "../lib/pugixml.hpp"
#include "TokenChecker.h"
#include "SessionsTable.h"

using namespace std;

const std::string UserLogout::state = "logout";

UserLogout::UserLogout(pugi::xml_document* doc): IMsgInLoginRequired(doc){
}

UserLogout::~UserLogout() {
}

int UserLogout::getMsgAuthorization() {
    return permissions::guest;
}

string UserLogout::createResponseMsgOut() {
    DAOMobileDevices::getInstance().deletepushNotification(_userId, "");
    SessionsTable::getInstance().removeEntry(_token);

    return getXMLreply(proto::replyTrue);
    
    //_outputMainNode.append_attribute(proto::errorCodeAttr) = ServerException::LOGOUT_ERROR;
    //return genOutputXMLwithVersionAndState(proto::replyFalse);
}




