/* 
 * File:   MsgInSwitch.h
 * Author: pavel
 *
 * Created on 22. červenec 2014, 10:15
 */

#ifndef MSGINSWITCH_H
#define	MSGINSWITCH_H

#include "IMsgInLoginAndAdapterAccessRequired.h"
#include "SocketClient.h"

class MsgInSwitch : public IMsgInLoginAndAdapterAccessRequired{
public:
    MsgInSwitch(char* msg, pugi::xml_document* doc);
    virtual ~MsgInSwitch();

    virtual int getMsgAuthorization();
    virtual string createResponseMsgOut();

    static const string state;

private:

};

#endif	/* MSGINSWITCH_H */
