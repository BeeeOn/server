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

using namespace std;

class MsgInGetConAccount : public IMsgInLoginAndAdapterAccessRequired{
public:
    MsgInGetConAccount(char* msg, pugi::xml_document* doc);
    virtual ~MsgInGetConAccount();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();
    
    static const std::string state;
private:
    
};

#endif	/* MSGINGETCONACCOUNT_H */

