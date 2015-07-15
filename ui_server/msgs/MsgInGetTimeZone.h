/* 
 * File:   MsgInGetTimeZone.h
 * Author: pavel
 *
 * Created on 1. červenec 2014, 12:04
 */

#ifndef MSGINGETTIMEZONE_H
#define	MSGINGETTIMEZONE_H

<<<<<<< HEAD
=======
#include "IMsgIn.h"
#include "../lib/pugixml.hpp"
>>>>>>> 5dc8f088d0b4585350a48f3f306f26862670287c
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

