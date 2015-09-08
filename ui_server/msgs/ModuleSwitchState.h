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

class ModuleSwitchState : public IMsgInLoginAndAdapterAccessRequired{
public:
    ModuleSwitchState(pugi::xml_document* doc);
    virtual ~ModuleSwitchState();

    virtual int getMsgAuthorization();
    virtual std::string createResponseMsgOut();

    static const std::string state;

private:

};

#endif	/* MSGINSWITCH_H */

