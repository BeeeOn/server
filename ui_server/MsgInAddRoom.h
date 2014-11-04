/* 
 * File:   MsgInAddRoom.h
 * Author: pavel
 *
 * Created on 24. červen 2014, 13:57
 */

#ifndef MSGINADDROOM_H
#define	MSGINADDROOM_H

#include "IMsgIn.h"
#include "pugixml.hpp"
#include "IMsgInLoginRequired.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"
class MsgInAddRoom : public IMsgInLoginAndAdapterAccessRequired{
public:
    MsgInAddRoom(char* msg, pugi::xml_document* doc);
    virtual ~MsgInAddRoom();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();

    static const std::string state;

};

#endif	/* MSGINADDROOM_H */

