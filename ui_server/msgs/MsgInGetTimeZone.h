/* 
 * File:   MsgInGetTimeZone.h
 * Author: pavel
 *
 * Created on 1. červenec 2014, 12:04
 */

#ifndef MSGINGETTIMEZONE_H
#define	MSGINGETTIMEZONE_H

#include "IMsgIn.h"
#include "../lib/pugixml.hpp"
#include "IMsgInLoginRequired.h"

class MsgInGetTimeZone : public IMsgInLoginRequired{
    public:
        MsgInGetTimeZone(pugi::xml_document* doc);
        virtual ~MsgInGetTimeZone();
        
        virtual int getMsgAuthorization();
        
        virtual std::string createResponseMsgOut();
        
        static const std::string state;
        private:
            
};

#endif	/* MSGINGETTIMEZONE_H */

