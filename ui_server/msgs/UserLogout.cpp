
#include "UserLogout.h"
#include "../DAO/DAOUsers.h"

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
    
    //TODO
    return getXMLreply(R_TRUE);
    
    //_outputMainNode.append_attribute(P_ERRCODE) = ServerException::LOGOUT_ERROR;
    //return genOutputXMLwithVersionAndState(R_FALSE);
}




