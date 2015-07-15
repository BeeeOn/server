/* 
 * File:   MsgInAddRoom.h
 * Author: pavel
 *
 * Created on 24. červen 2014, 13:57
 */

#ifndef MSGINADDROOM_H
#define	MSGINADDROOM_H

#include "IMsgIn.h"
#include "../lib/pugixml.hpp"
#include "IMsgInLoginRequired.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"
class MsgInAddRoom : public IMsgInLoginAndAdapterAccessRequired{
public:
    MsgInAddRoom(pugi::xml_document* doc);
    virtual ~MsgInAddRoom();

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();

    static const std::string state;

};

#endif	/* MSGINADDROOM_H */

