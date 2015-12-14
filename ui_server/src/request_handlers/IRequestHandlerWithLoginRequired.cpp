#include "IRequestHandlerWithLoginRequired.h"
#include "../DAO/DAOUsers.h"
#include "IRequestHandlerWithoutUac.h"
#include "SessionsTable.h"

using namespace std;

IRequestHandlerWithLoginRequired::IRequestHandlerWithLoginRequired(shared_ptr<pugi::xml_document> doc, SessionsTable & sessionTable)
    :   IRequestHandlerWithoutUac(doc),
        _sessionTable(sessionTable)
{
    _userId = -1;
}

IRequestHandlerWithLoginRequired::~IRequestHandlerWithLoginRequired() 
{
}

bool IRequestHandlerWithLoginRequired::isComIdValid()
{
    
    _userId = _sessionTable.getUserIdBySessionString(_token);
    
    Logger::getInstance(Logger::DEBUG)<<"user:"<<_userId<<" token:"<<_token<<endl;
    
    return (_userId > 0 );
}

enumAccessStatus IRequestHandlerWithLoginRequired::checkAccess()
{
    if( isComIdValid() )
        return GRANTED;
    else
        return FORBIDDEN_NOT_LOGGED;
}
