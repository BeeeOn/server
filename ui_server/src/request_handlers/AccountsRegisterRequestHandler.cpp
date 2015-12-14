#include "AccountsRegisterRequestHandler.h"
#include "ServerException.h"
#include "../DAO/DAOUsers.h"
#include "TokenChecker.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
using namespace std;

const string AccountsRegisterRequestHandler::state = "register";

AccountsRegisterRequestHandler::AccountsRegisterRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers)
    :   IRequestHandlerWithoutUac(doc),
        _daoUsers(daoUsers)
{
}

AccountsRegisterRequestHandler::~AccountsRegisterRequestHandler(void)
{
}

int AccountsRegisterRequestHandler::getMsgAuthorization() {
    return permissions::everyone;
}

string AccountsRegisterRequestHandler::createResponseMsgOut()
{
    pugi::xml_node authProviderNode =  _doc->child(proto::requestNode).child(proto::authProviderNode);
    
    string service = authProviderNode.attribute(proto::authProvideNameAttr).value();
    
    User user;
    string token;
    
    if(service == "beeeon")
    {
        string beeeonMail = authProviderNode.attribute(proto::authProvideMailAttr).value();  
        string beeeonPassword = authProviderNode.attribute(proto::authProvidePasswordAttr).value();  
        user.mail = beeeonMail;
        user.password = beeeonPassword;
    }
    else
    {
        if(service == "google")
        {
            token = authProviderNode.attribute(proto::authProvideTokenAttr).value();      
        }
        else if(service == "facebook")
        {
            token = authProviderNode.attribute(proto::authProvideTokenAttr).value();  
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
    }
    
    if(_daoUsers.add(user) == 0)
    {
        return getNegativeXMLReply(ServerException::IDENTIFICATION_TAKEN);
    }

    return getXMLreply(proto::replyTrue);
}
