/* 
 * File:   MsgInGetConAccount.h
 * Author: pavel
 *
 * Created on 2. červenec 2014, 11:23
 */

#ifndef MSGINGETCONACCOUNT_H
#define	MSGINGETCONACCOUNT_H

#include "IMsgIn.h"
#include "IMsgInLoginRequired.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"


class AccountGet : public IMsgInLoginAndAdapterAccessRequired{
public:
    AccountGet(pugi::xml_document* doc);
    virtual ~AccountGet();

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();
    
    static const std::string state;
private:
    
};

#endif	/* MSGINGETCONACCOUNT_H */

