/* 
 * File:   MsgInDelConAccount.h
 * Author: pavel
 *
 * Created on 2. červenec 2014, 11:32
 */

#ifndef MSGINDELCONACCOUNT_H
#define	MSGINDELCONACCOUNT_H

#include "IMsgInLoginAndAdapterAccessRequired.h"

class AccountDel : public IMsgInLoginAndAdapterAccessRequired{
    public:
        AccountDel(pugi::xml_document* doc);
        virtual ~AccountDel();       

        virtual int getMsgAuthorization();

        virtual std::string createResponseMsgOut();
        
        static const std::string state;
    private:
            
};

#endif	/* MSGINDELCONACCOUNT_H */

