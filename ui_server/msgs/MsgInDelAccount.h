/* 
 * File:   MsgInDelConAccount.h
 * Author: pavel
 *
 * Created on 2. červenec 2014, 11:32
 */

#ifndef MSGINDELCONACCOUNT_H
#define	MSGINDELCONACCOUNT_H

#include "IMsgIn.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"
#include "DBConnector.h"


class MsgInDelConAccount : public IMsgInLoginAndAdapterAccessRequired{
    public:
        MsgInDelConAccount(pugi::xml_document* doc);
        virtual ~MsgInDelConAccount();       

    virtual int getMsgAuthorization();

        virtual string createResponseMsgOut();
        
        static const std::string state;
        private:
            
};

#endif	/* MSGINDELCONACCOUNT_H */

