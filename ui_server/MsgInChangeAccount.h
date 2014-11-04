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

using namespace std;

class MsgInChangeConAccount : public IMsgInLoginAndAdapterAccessRequired {
public:
    MsgInChangeConAccount(char* msg, pugi::xml_document* doc);
    virtual ~MsgInChangeConAccount();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();
    
    static const std::string state;
private:

};

#endif	/* MSGINCHANGECONACCOUNT_H */

