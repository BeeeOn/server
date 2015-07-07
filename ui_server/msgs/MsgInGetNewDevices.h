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
    MsgInGetNewDevices(pugi::xml_document* doc);
    virtual ~MsgInGetNewDevices();

    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();

    static const std::string state;

private:

};

#endif	/* MSGINGETNEWDEVICES_H */

