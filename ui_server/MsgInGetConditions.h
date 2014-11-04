/* 
 * File:   MsgInGetConditions.h
 * Author: pavel
 *
 * Created on 16. září 2014, 15:23
 */

#ifndef MSGINGETCONDITIONS_H
#define	MSGINGETCONDITIONS_H

#include "IMsgInLoginRequired.h"


class MsgInGetConditions : public IMsgInLoginRequired{
public:
    MsgInGetConditions(char* msg, pugi::xml_document* doc);
    virtual ~MsgInGetConditions();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();

    static const string state;
private:

};

#endif	/* MSGINGETCONDITIONS_H */

