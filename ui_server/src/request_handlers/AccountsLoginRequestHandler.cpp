#include "AccountsLoginRequestHandler.h"
#include "ServerException.h"
#include "../DAO/DAOUsers.h"
#include "../DAO/DAOMobileDevices.h"
#include "SessionsTable.h"
#include "TokenChecker.h"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

using namespace std;
const std::string AccountsLoginRequestHandler::state = "login";

AccountsLoginRequestHandler::AccountsLoginRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, SessionsTable & sessionTable)
    :   IRequestHandlerWithoutUac(doc),
        _daoUsers(daoUsers),
        _sessionTable(sessionTable)
{
}

AccountsLoginRequestHandler::~AccountsLoginRequestHandler(void)
{
}

int AccountsLoginRequestHandler::getMsgAuthorization() 
{
    return permissions::everyone;
}

string AccountsLoginRequestHandler::createResponseMsgOut()
{
    Logger::debug()<<"loging in"<<endl;
    pugi::xml_node providerNode =  _doc->child(proto::requestNode).child(proto::authProviderNode);
    
    string service = providerNode.attribute(proto::authProvideNameAttr).value();
    
    string phoneId =  _doc->child(proto::requestNode).child(proto::phoneNode).attribute(proto::phoneNameAttr).value();
            
    MobileDevice mobile;
    mobile.locale = "";
    mobile.mobile_id = phoneId;
    mobile.push_notification = "";

    mobile.type = "android";
    User user;
    
    string providerToken;
    if(service == "beeeon")
    {
        string beeeonMail = providerNode.attribute(proto::authProvideMailAttr).value();  
        string beeeonPassword = providerNode.attribute(proto::authProvidePasswordAttr).value();  
        user.mail = beeeonMail;
        user.password = beeeonPassword;
    }
    else
    {
       if(service == "google")
        {
            providerToken = providerNode.attribute(proto::authProvideTokenAttr).value();    
        }
        else if(service == "facebook")
        {
            providerToken = providerNode.attribute(proto::authProvideTokenAttr).value(); 
        }
        else
        {
            Logger::error() << "unsupported provider : " << service <<endl;
            return getNegativeXMLReply(ServerException::WRONG_AUTH_PROVIDER);
        }

        TokenChecker tokenChecker(service);

        if( !tokenChecker.isTokenOkay(providerToken) )
        {
            return getNegativeXMLReply(ServerException::TOKEN_EMAIL);
        }

        if ( tokenChecker.email == "" )
        {
            _outputMainNode.append_attribute(proto::errorCodeAttr) = ServerException::NO_MAIL_PROVIDED;
            return getXMLreply(proto::replyFalse);
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
    }
    
    int userId = _daoUsers.getUserIDbyAlternativeKeys(user.mail, user.password, user.googleId, user.facebookId);
    
    if(userId < 0)
    {
        _outputMainNode.append_attribute(proto::errorCodeAttr) = ServerException::USER_DONOT_EXISTS;
        return getXMLreply(proto::replyFalse);
    }
    
    string token = _sessionTable.addNewSession(userId, user.mail, phoneId);
    pugi::xml_node sessionNode = _outputMainNode.append_child("session");
    sessionNode.append_attribute("id") = token.c_str();
    return getXMLreply(proto::replySessionId);
}
