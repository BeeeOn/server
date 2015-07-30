/* 
 * File:   MsgInAddAdapter.h
 * Author: pavel
 *
 * Created on 3. září 2014, 13:11
 */

#ifndef MSGINADDADAPTER_H
#define	MSGINADDADAPTER_H

#include "IMsgInLoginRequired.h"
#include "../DAO/DAOAdapters.h"
#include "../DAO/DAOUsersAdapters.h"
#include "IMsgIn.h"

class GateAdd : public IMsgInLoginRequired{
public:
    GateAdd(pugi::xml_document* doc);
    virtual ~GateAdd();
    virtual int getMsgAuthorization();
    virtual std::string createResponseMsgOut();
    
    static const std::string state;

private:

};

#endif	/* MSGINADDADAPTER_H */

