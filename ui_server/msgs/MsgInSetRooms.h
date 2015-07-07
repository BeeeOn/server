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


class MsgInSetRooms:
	public IMsgInLoginAndAdapterAccessRequired {
public:
    MsgInSetRooms(pugi::xml_document* doc);
    virtual ~MsgInSetRooms();

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();
    
    static const std::string state;
    
private:

};

#endif	/* MSGINUPDATEROOMS_H */

