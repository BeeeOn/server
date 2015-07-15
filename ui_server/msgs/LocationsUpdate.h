/* 
 * File:   MsgInUpdateRooms.h
 * Author: pavel
 *
 * Created on 23. červen 2014, 11:03
 */

#ifndef MSGINUPDATEROOMS_H
#define	MSGINUPDATEROOMS_H

#include "IMsgIn.h"
#include "DBConnector.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"


class LocationsUpdate:
	public IMsgInLoginAndAdapterAccessRequired {
public:
    LocationsUpdate(pugi::xml_document* doc);
    virtual ~LocationsUpdate();

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();
    
    static const std::string state;
    
private:

};

#endif	/* MSGINUPDATEROOMS_H */

