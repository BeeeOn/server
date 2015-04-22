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


class MsgInUpdateRooms:
	public IMsgInLoginAndAdapterAccessRequired {
public:
    MsgInUpdateRooms(char* msg, pugi::xml_document* doc);
    virtual ~MsgInUpdateRooms();

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();
    
    static const std::string state;
    
private:

};

#endif	/* MSGINUPDATEROOMS_H */

