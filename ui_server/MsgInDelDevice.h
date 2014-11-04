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
    MsgInDelDevice(char* msg, pugi::xml_document* doc);
    virtual ~MsgInDelDevice();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();

    static const string state;
    private:

};

#endif	/* MSGINDELDEVICE_H */

