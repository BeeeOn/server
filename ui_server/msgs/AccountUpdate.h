/* 
 * File:   MsgInChangeConAccount.h
 * Author: pavel
 *
 * Created on 2. červenec 2014, 13:43
 */

#ifndef MSGINCHANGECONACCOUNT_H
#define	MSGINCHANGECONACCOUNT_H

#include "IMsgIn.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"
#include "DBConnector.h"


class AccountUpdate : public IMsgInLoginAndAdapterAccessRequired {
public:
    AccountUpdate(pugi::xml_document* doc);
    virtual ~AccountUpdate();

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();
    
    static const std::string state;
private:

};

#endif	/* MSGINCHANGECONACCOUNT_H */

