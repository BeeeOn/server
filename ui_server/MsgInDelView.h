/* 
 * File:   MsgInDelView.h
 * Author: pavel
 *
 * Created on 2. červenec 2014, 16:50
 */

#ifndef MSGINDELVIEW_H
#define	MSGINDELVIEW_H

#include "IMsgIn.h"
#include "IMsgInLoginUnwanted.h"
#include "IMsgInLoginRequired.h"
#include "DBConnector.h"

class MsgInDelView : public IMsgInLoginRequired{
    public:
        MsgInDelView(char* msg, pugi::xml_document* doc);
        virtual ~MsgInDelView();
        
        virtual int getMsgAuthorization();
        virtual string createResponseMsgOut();
        
        static const std::string state;
        private:
            
};

#endif	/* MSGINDELVIEW_H */

