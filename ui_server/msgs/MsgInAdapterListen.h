/* 
 * File:   MsgInAdapterListen.h
 * Author: pavel
 *
 * Created on 22. červenec 2014, 10:14
 */

#ifndef MSGINADAPTERLISTEN_H
#define	MSGINADAPTERLISTEN_H

#include "IMsgInLoginAndAdapterAccessRequired.h"

#include "SocketClient.h"

class MsgInAdapterListen : public IMsgInLoginAndAdapterAccessRequired{
    public:
        MsgInAdapterListen(pugi::xml_document* doc);
        virtual ~MsgInAdapterListen();
        
        virtual int getMsgAuthorization();

        virtual std::string createResponseMsgOut();

        static const std::string state;
        private:
            
};

#endif	/* MSGINADAPTERLISTEN_H */

