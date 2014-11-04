/* 
 * File:   MsgInAddView.h
 * Author: pavel
 *
 * Created on 2. červenec 2014, 15:00
 */

#ifndef MSGINADDVIEW_H
#define	MSGINADDVIEW_H

#include "IMsgIn.h"
#include "ServerException.h"
#include "IMsgInLoginRequired.h"
#include "DBConnector.h"

class MsgInAddView : public IMsgInLoginRequired{
    public:
        MsgInAddView(char* msg, pugi::xml_document* doc);
        virtual ~MsgInAddView();
        
        virtual int getMsgAuthorization();
        virtual string createResponseMsgOut();
        
        static const std::string state;
        private:
            
};

#endif	/* MSGINADDVIEW_H */

