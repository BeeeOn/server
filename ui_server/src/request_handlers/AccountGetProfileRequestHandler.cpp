#include "AccountGetProfileRequestHandler.h"
#include "../DAO/DAOUsers.h"
#include "../lib/pugixml.hpp"

using namespace std;

const std::string AccountGetProfileRequestHandler::state = "getmyprofile";

AccountGetProfileRequestHandler::AccountGetProfileRequestHandler(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginRequired(doc, sessionTable),
        _daoUsers(daoUsers)
{
}


AccountGetProfileRequestHandler::~AccountGetProfileRequestHandler() 
{
}

int AccountGetProfileRequestHandler::getMsgAuthorization() 
{
    return permissions::guest;
}

string AccountGetProfileRequestHandler::createResponseMsgOut() 
{
    
    User user = _daoUsers.getUserByID(_userId);
    cout << _userId << "asdas"<< user.user_id<<endl;
    pugi::xml_node userNode = _outputMainNode.append_child();
    userNode.set_name(proto::userNode);
    userNode.append_attribute(proto::userIdAttr) = user.user_id;
    userNode.append_attribute(proto::userNameAttr) = user.givenName.c_str();
    userNode.append_attribute(proto::userSurnameAttr) = user.familyName.c_str();
    userNode.append_attribute(proto::userGenderAttr) = user.gender.c_str();
    userNode.append_attribute(proto::userEmailAttr) = user.mail.c_str();
    userNode.append_attribute(proto::userPictureAttr) = user.picture.c_str();

    pugi::xml_node accountsNode = _outputMainNode.append_child();
    accountsNode.set_name(proto::userAccountsAttr);
    
    pugi::xml_node serviceNode;
    
    if(user.googleId != "")
    {
        serviceNode = accountsNode.append_child();
        serviceNode.set_name(proto::authProviderNode);
        serviceNode.append_attribute(proto::authProvideNameAttr) = proto::authServiceGoogleAttr;
        serviceNode.append_attribute(proto::authProviderIdAttr) = user.googleId.c_str();
    }
    
    if(user.facebookId != "")
    {
        serviceNode = accountsNode.append_child();
        serviceNode.set_name(proto::authProviderNode);
        serviceNode.append_attribute(proto::authProvideNameAttr) = proto::authServiceFacebookAttr;
        serviceNode.append_attribute(proto::authProviderIdAttr) = user.facebookId.c_str();
    }
    
    return getXMLreply(proto::replyUserInfo);
}
