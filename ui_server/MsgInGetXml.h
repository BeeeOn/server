/* 
 * File:   MsgInGetXml.h
 * Author: pavel
 *
 * Created on 3. červenec 2014, 13:03
 */

#ifndef MSGINGETXML_H
#define	MSGINGETXML_H

#include "IMsgIn.h"
#include "MsgOutCommunication.h"
#include "DBConnector.h"
#include "ComTable.h"
#include "MsgOutFalse.h"
#include "MsgOutReSign.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

class MsgInGetXml :public IMsgInLoginAndAdapterAccessRequired{
public:
    MsgInGetXml(char* msg, pugi::xml_document* doc);
    virtual ~MsgInGetXml();
    virtual IMsgOut* createResponseMsgOut();
    
    static const std::string state;
    private:

};

#endif	/* MSGINGETXML_H */

