
#include "UserRegister.h"
#include "ServerException.h"
#include "../DAO/DAOUsers.h"
#include "TokenChecker.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
const std::string UserRegister::state = "signup";


UserRegister::UserRegister(pugi::xml_document* doc): IMsgInFreeAccess(doc)
{
}

UserRegister::~UserRegister(void)
{
}

int UserRegister::getMsgAuthorization() {
    return EVERYONE;
}

string UserRegister::createResponseMsgOut()
{
    pugi::xml_node parametersNode =  _doc->child(P_COMMUNICATION).child(P_SIGN_PARAMS);
    
    string service = _doc->child(P_COMMUNICATION).attribute(P_SIGN_SERVICE).value();
    
    User user;
    string token;
    if(service == "google")
    {
        token = parametersNode.attribute(P_GOOGLE_TOKEN).value();      
    }
    else if(service == "facebook")
    {
        token = parametersNode.attribute(P_FACEBOOK_TOKEN).value();  
    }
    else
    {
        Logger::error() << "unsupported provider : " << service <<endl;
        return getNegativeXMLReply(ServerException::WRONG_AUTH_PROVIDER);
    }
    
    TokenChecker tokenChecker(service);
    
    if( !tokenChecker.isTokenOkay(token) )
    {
        return getNegativeXMLReply(ServerException::TOKEN_EMAIL);
    }
    
    if(service == "google")
    {
        user.googleId = tokenChecker.id;    
    }
    else if(service == "facebook")
    {
        user.facebookId = tokenChecker.id;
    }
    
    user.familyName = tokenChecker.family_name;
    user.gender = tokenChecker.gender;
    user.givenName = tokenChecker.given_name;
    
    user.mail = tokenChecker.email;
    user.picture = tokenChecker.picture;  
    
    
    if(DAOUsers::getInstance().add(user) == 0)
    {
        return getNegativeXMLReply(ServerException::IDENTIFICATION_TAKEN);
    }

    return getXMLreply(R_TRUE);
}
