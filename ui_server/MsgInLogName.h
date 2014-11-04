/* 
 * File:   MsgInLogName.h
 * Author: pavel
 *
 * Created on 23. červen 2014, 11:57
 */

#ifndef MSGINLOGNAME_H
#define	MSGINLOGNAME_H

#include "IMsgIn.h"
#include "pugixml.hpp"
#include "MsgOutReSign.h"
#include "MsgOutLogContent.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

using namespace std;

class MsgInLogName : public IMsgInLoginAndAdapterAccessRequired {
public:
    MsgInLogName(char* msg, pugi::xml_document* doc);
    virtual ~MsgInLogName();
    virtual IMsgOut* createResponseMsgOut();
    
    static const std::string state;

};

#endif	/* MSGINLOGNAME_H */

