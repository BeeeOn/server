#include "IRequestHandlerWithLoginAndGateAccessRequired.h"
#include "../DAO/DAOUsers.h"

using namespace std;

IRequestHandlerWithLoginAndGateAccessRequired::IRequestHandlerWithLoginAndGateAccessRequired(shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, SessionsTable & sessionTable)
    :   IRequestHandlerWithLoginRequired(doc, sessionTable),
        _daoUsers(daoUsers)
{
    _gatewayId = _doc->child(proto::requestNode).attribute(proto::headerGatewayIdAttr).as_ullong(0);
}

IRequestHandlerWithLoginAndGateAccessRequired::~IRequestHandlerWithLoginAndGateAccessRequired() 
{
}

enumAccessStatus IRequestHandlerWithLoginAndGateAccessRequired::checkAccess()
{
    if( !isComIdValid() )
        return FORBIDDEN_NOT_LOGGED;
    
    std::string role = _daoUsers.getUserRoleM(_userId, _gatewayId);
    
    int roleId;
    if(role == proto::roleGuestAttr)
        roleId= permissions::guest;
    else if(role == proto::roleUserAttr)
        roleId= permissions::user;
    else if(role == proto::roleAdminAttr)
        roleId= permissions::admin;
    else if(role == proto::roleOwnerAttr)
        roleId= permissions::owner;
    else {
        roleId = -1;
        Logger::getInstance(Logger::ERROR) << "undefined role:>"<<role <<"< ";
    }
    Logger::getInstance(Logger::DEBUG3) << "check role: "<< _userId <<" on "<<_state<<"("<<role<<"="<<roleId<<" "<<_state<<"="<<this->getMsgAuthorization() <<")"<<endl;

    _role = role;
    
    if(roleId >= this->getMsgAuthorization())
    {
         Logger::getInstance(Logger::DEBUG3) << "msg right OK "<<endl;
         return GRANTED;
    }
    else
    {
        Logger::getInstance(Logger::ERROR) << " NOT OK "<< _userId <<" on "<<_state<<"("<<role<<"="<<roleId<<" "<<_state<<"="<<this->getMsgAuthorization() <<") "<<endl;
        return FORBIDDEN_WRONG_RIGHTS;
    }
}
