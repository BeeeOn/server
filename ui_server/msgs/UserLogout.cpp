
#include "UserLogout.h"
#include "../DAO/DAOUsers.h"
#include "../lib/pugixml.hpp"
#include "TokenChecker.h"

using namespace std;

const std::string UserLogout::state = "logout";

UserLogout::UserLogout(pugi::xml_document* doc): IMsgInLoginRequired(doc){
}

UserLogout::~UserLogout() {
}

int UserLogout::getMsgAuthorization() {
    return GUEST;
}

string UserLogout::createResponseMsgOut() {
    DAOMobileDevices::getInstance().deletepushNotification(_userId, "");

    return getXMLreply(R_TRUE);
    
    //_outputMainNode.append_attribute(P_ERRCODE) = ServerException::LOGOUT_ERROR;
    //return genOutputXMLwithVersionAndState(R_FALSE);
}




