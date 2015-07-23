/* 
 * File:   MsgInPartial.h
 * Author: pavel
 *
 * Created on 3. červen 2014, 8:03
 */

#ifndef MSGINPARTIALL_H
#define	MSGINPARTIALL_H

#include "../lib/pugixml.hpp"
#include <string>
#include <iostream>
#include "DBConnector.h"
#include "IMsgIn.h"
#include "IMsgInLoginRequired.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

class DevicesUpdate : public IMsgInLoginAndAdapterAccessRequired {
public:
    DevicesUpdate(pugi::xml_document* doc);
    virtual ~DevicesUpdate();
    

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();

    static const std::string state;
private:

};

#endif	/* MSGINPARTIALL_H */

