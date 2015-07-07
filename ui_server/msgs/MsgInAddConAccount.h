/* 
 * File:   MsgInAddConAccount.h
 * Author: pavel
 *
 * Created on 2. červenec 2014, 14:06
 */

#ifndef MSGINADDCONACCOUNT_H
#define	MSGINADDCONACCOUNT_H

#include "IMsgIn.h"
#include "MsgOutReSign.h"
#include "MsgOutTrue.h"
#include "IMsgInLoginRequired.h"
#include "DBConnector.h"

class MsgInAddConAccount :public IMsgInLoginRequired {
public:
    MsgInAddConAccount(char* msg, pugi::xml_document* doc);
    virtual ~MsgInAddConAccount();
    virtual IMsgOut* createResponseMsgOut();
    
    static const std::string state;
private:

};

#endif	/* MSGINADDCONACCOUNT_H */

