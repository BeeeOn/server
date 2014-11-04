/* 
 * File:   MsgInDelConAccount.h
 * Author: pavel
 *
 * Created on 2. červenec 2014, 11:32
 */

#ifndef MSGINDELCONACCOUNT_H
#define	MSGINDELCONACCOUNT_H

#include "IMsgIn.h"
#include "IMsgInLoginRequired.h"
#include "DBConnector.h"


class MsgInDelConAccount : public IMsgInLoginRequired{
    public:
        MsgInDelConAccount(char* msg, pugi::xml_document* doc);
        virtual ~MsgInDelConAccount();       
        virtual IMsgOut* createResponseMsgOut();
        
        static const std::string state;
        private:
            
};

#endif	/* MSGINDELCONACCOUNT_H */

