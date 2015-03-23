/* 
 * File:   MsgInAddAdapter.h
 * Author: pavel
 *
 * Created on 3. září 2014, 13:11
 */

#ifndef MSGINADDADAPTER_H
#define	MSGINADDADAPTER_H

#include "IMsgInLoginRequired.h"
#include "DBConnector.h"
#include "../DAO/DAOAdapters.h"
#include "IMsgIn.h"

class MsgInAddAdapter : public IMsgInLoginRequired{
public:
    MsgInAddAdapter(char* msg, pugi::xml_document* doc);
    virtual ~MsgInAddAdapter();
    virtual int getMsgAuthorization();
    virtual string createResponseMsgOut();
    
    static const std::string state;

private:

};

#endif	/* MSGINADDADAPTER_H */

