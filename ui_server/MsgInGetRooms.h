/* 
 * File:   MsgInGetRooms.h
 * Author: pavel
 *
 * Created on 23. červen 2014, 11:23
 */

#ifndef MSGINGETROOMS_H
#define	MSGINGETROOMS_H

#include "IMsgIn.h"
#include "IMsgInLoginRequired.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

using namespace std;

class MsgInGetRooms : public IMsgInLoginAndAdapterAccessRequired{
public:
    MsgInGetRooms(char* msg, pugi::xml_document* doc);
    virtual ~MsgInGetRooms();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();
    
    static const std::string state;
    
private:

};

#endif	/* MSGINGETROOMS_H */

