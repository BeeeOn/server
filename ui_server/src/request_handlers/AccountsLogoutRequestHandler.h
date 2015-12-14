#ifndef USERLOGOUT_H
#define	USERLOGOUT_H

#include "IRequestHandlerWithLoginRequired.h"
#include "DAO/DAOMobileDevices.h"
#include "SessionsTable.h"

class AccountsLogoutRequestHandler
    :   public IRequestHandlerWithLoginRequired 
{
public:
    AccountsLogoutRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOMobileDevices & daoMobileDevices, SessionsTable & sessionTable);
    virtual ~AccountsLogoutRequestHandler();
    virtual std::string createResponseMsgOut();
    virtual int getMsgAuthorization();
    static const std::string state;
private:
    DAOMobileDevices & _daoMobileDevices;
        
};

#endif	/* USERLOGOUT_H */

