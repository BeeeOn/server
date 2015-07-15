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

class LocationsGet : public IMsgInLoginAndAdapterAccessRequired{
public:
    LocationsGet(pugi::xml_document* doc);
    virtual ~LocationsGet();

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();
    
    static const std::string state;
    
private:

};

#endif	/* MSGINGETROOMS_H */

