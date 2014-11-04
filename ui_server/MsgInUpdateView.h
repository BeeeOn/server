/* 
 * File:   MsgInUpdateView.h
 * Author: pavel
 *
 * Created on 2. červenec 2014, 16:35
 */

#ifndef MSGINUPDATEVIEW_H
#define	MSGINUPDATEVIEW_H

#include "IMsgIn.h"
#include "ServerException.h"
#include "IMsgInLoginRequired.h"
#include "DBConnector.h"

class MsgInUpdateView : public IMsgInLoginRequired{
    public:
        MsgInUpdateView(char* msg, pugi::xml_document* doc);
        virtual ~MsgInUpdateView();
        
        virtual int getMsgAuthorization();
        virtual string createResponseMsgOut();
        
        static const std::string state;
        private:
            
};

#endif	/* MSGINUPDATEVIEW_H */

