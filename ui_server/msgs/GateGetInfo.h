/* 
 * File:   GateInfo
 * Author: pavel
 *
 * Created on 1. červenec 2014, 12:04
 */

#ifndef GATEINFO_H
#define	GATEINFO_H

#include "IMsgIn.h"
#include "pugixml.hpp"
#include "IMsgInLoginAndAdapterAccessRequired.h"

class GateGetInfo : public IMsgInLoginAndAdapterAccessRequired{
    public:
        GateGetInfo(pugi::xml_document* doc);
        virtual ~GateGetInfo();
        
        virtual int getMsgAuthorization();
        
        virtual std::string createResponseMsgOut();
        
        static const std::string state;
        private:
            
};

#endif	/* GATEINFO_H */

