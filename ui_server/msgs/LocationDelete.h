/* 
 * File:   MsgInDelRoom.h
 * Author: pavel
 *
 * Created on 24. červen 2014, 14:48
 */

#ifndef MSGINDELROOM_H
#define	MSGINDELROOM_H

#include "IMsgIn.h"
#include "DBConnector.h"
#include "IMsgInLoginRequired.h"


class LocationDelete : public IMsgInLoginRequired{
public:
    LocationDelete(pugi::xml_document* doc);
    virtual ~LocationDelete();

    virtual int getMsgAuthorization();

    
    virtual std::string createResponseMsgOut();

    static const std::string state;

};

#endif	/* MSGINDELROOM_H */

