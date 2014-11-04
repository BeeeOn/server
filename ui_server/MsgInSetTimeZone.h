/* 
 * File:   MsgInSetTimeZone.h
 * Author: pavel
 *
 * Created on 1. červenec 2014, 11:23
 */

#ifndef MSGINSETTIMEZONE_H
#define	MSGINSETTIMEZONE_H
#include "IMsgIn.h"
#include "pugixml.hpp"
#include "DBConnector.h"
#include "IMsgInLoginRequired.h"

using namespace std;

class MsgInSetTimeZone : public IMsgInLoginRequired{
public:
    MsgInSetTimeZone(char* msg, pugi::xml_document* doc);
    virtual ~MsgInSetTimeZone();

    virtual int getMsgAuthorization();
    
    virtual string createResponseMsgOut();
        
    static const std::string state;
private:

};

#endif	/* MSGINSETTIMEZONE_H */

