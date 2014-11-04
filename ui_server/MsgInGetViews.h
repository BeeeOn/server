/* 
 * File:   MsgInGetViews.h
 * Author: pavel
 *
 * Created on 2. červenec 2014, 17:03
 */

#ifndef MSGINGETVIEWS_H
#define	MSGINGETVIEWS_H

#include "IMsgIn.h"
#include "IMsgInLoginUnwanted.h"
#include "IMsgInLoginRequired.h"

class MsgInGetViews : public IMsgInLoginRequired{
    public:
        MsgInGetViews(char* msg, pugi::xml_document* doc);
        
        virtual ~MsgInGetViews();
        
        virtual int getMsgAuthorization();
        virtual string createResponseMsgOut();
        
        static const std::string state;
        private:
            
};

#endif	/* MSGINGETVIEWS_H */

