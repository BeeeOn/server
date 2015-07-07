/* 
 * File:   MsgInDelDevice.h
 * Author: pavel
 *
 * Created on 5. srpen 2014, 10:27
 */

#ifndef MSGINDELDEVICE_H
#define	MSGINDELDEVICE_H

#include "IMsgInLoginAndAdapterAccessRequired.h"
#include "SocketClient.h"

class MsgInDelDevice:public IMsgInLoginAndAdapterAccessRequired{
public:
    MsgInDelDevice(pugi::xml_document* doc);
    virtual ~MsgInDelDevice();

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();

    static const std::string state;
    private:

};

#endif	/* MSGINDELDEVICE_H */

