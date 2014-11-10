/* 
 * File:   MsgInGetNewDevices.h
 * Author: pavel
 *
 * Created on 6. říjen 2014, 13:27
 */

#ifndef MSGINGETNEWDEVICES_H
#define	MSGINGETNEWDEVICES_H

#include "IMsgInLoginAndAdapterAccessRequired.h"


class MsgInGetNewDevices : public IMsgInLoginAndAdapterAccessRequired {
public:
    MsgInGetNewDevices(char* msg, pugi::xml_document* doc);
    virtual ~MsgInGetNewDevices();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();

    static const string state;

private:

};

#endif	/* MSGINGETNEWDEVICES_H */
