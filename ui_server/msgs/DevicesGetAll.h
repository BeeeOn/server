/* 
 * File:   MsgInGetXml.h
 * Author: pavel
 *
 * Created on 3. červenec 2014, 13:03
 */

#ifndef MSGINGETXML_H
#define	MSGINGETXML_H

#include "IMsgIn.h"
#include "DBConnector.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

class DevicesGetAll :public IMsgInLoginAndAdapterAccessRequired{
public:
    DevicesGetAll(pugi::xml_document* doc);
    virtual ~DevicesGetAll();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();
    
    static const std::string state;
    private:

};

#endif	/* MSGINGETXML_H */

