/* 
 * File:   MsgInLogName.h
 * Author: pavel
 *
 * Created on 23. červen 2014, 11:57
 */

#ifndef MSGINLOGNAME_H
#define	MSGINLOGNAME_H

#include "IMsgIn.h"
#include "../lib/pugixml.hpp"
#include "IMsgInLoginAndAdapterAccessRequired.h"

class GetLog : public IMsgInLoginAndAdapterAccessRequired {
public:
    GetLog(pugi::xml_document* doc);
    virtual ~GetLog();

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();
    
    static const std::string state;

};

#endif	/* MSGINLOGNAME_H */

