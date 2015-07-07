/* 
 * File:   MsgInSetCondition.h
 * Author: pavel
 *
 * Created on 16. září 2014, 9:27
 */

#ifndef MSGINSETCONDITION_H
#define	MSGINSETCONDITION_H

#include "IMsgInLoginRequired.h"


class MsgInSetCondition: public IMsgInLoginRequired {
public:
    MsgInSetCondition(char* msg, pugi::xml_document* doc);
    virtual ~MsgInSetCondition();

    virtual string createResponseMsgOut();
    virtual int getMsgAuthorization();

    static const string state;
};

#endif	/* MSGINSETCONDITION_H */

