/* 
 * File:   msgInGetCondition.h
 * Author: pavel
 *
 * Created on 16. září 2014, 11:38
 */

#ifndef MSGINGETCONDITION_H
#define	MSGINGETCONDITION_H

#include "IMsgInLoginRequired.h"


class msgInGetCondition : public IMsgInLoginRequired{
public:
    msgInGetCondition(char* msg, pugi::xml_document* doc);
    virtual ~msgInGetCondition();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();

    static const string state;
private:
};

#endif	/* MSGINGETCONDITION_H */

