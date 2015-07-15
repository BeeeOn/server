
#include "UserLogout.h"
#include "../DAO/DAOUsers.h"
<<<<<<< HEAD
=======
#include "../lib/pugixml.hpp"
>>>>>>> 5dc8f088d0b4585350a48f3f306f26862670287c

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




