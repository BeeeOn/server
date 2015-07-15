/* 
 * File:   MsgInDelRoom.h
 * Author: pavel
 *
 * Created on 24. červen 2014, 14:48
 */

#ifndef MSGINDELROOM_H
#define	MSGINDELROOM_H

#include "IMsgIn.h"
#include "../lib/pugixml.hpp"
#include "DBConnector.h"
#include "IMsgInLoginRequired.h"


class MsgInDelRoom : public IMsgInLoginRequired{
public:
    MsgInDelRoom(pugi::xml_document* doc);
    virtual ~MsgInDelRoom();

    virtual int getMsgAuthorization();

    
    virtual std::string createResponseMsgOut();

    static const std::string state;

};

#endif	/* MSGINDELROOM_H */

