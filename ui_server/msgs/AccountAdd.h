/* 
 * File:   MsgInAddConAccount.h
 * Author: pavel
 *
 * Created on 2. červenec 2014, 14:06
 */

#ifndef MSGINADDCONACCOUNT_H
#define	MSGINADDCONACCOUNT_H

#include "IMsgIn.h"
#include "DBConnector.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

class MsgInAddAccount :public IMsgInLoginAndAdapterAccessRequired {
public:
    MsgInAddAccount(pugi::xml_document* doc);
    virtual ~MsgInAddAccount();
    virtual int getMsgAuthorization();
    virtual string createResponseMsgOut();
    
    static const std::string state;
private:

};

#endif	/* MSGINADDCONACCOUNT_H */

