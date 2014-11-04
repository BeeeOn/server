/* 
 * File:   MsgInDelCondition.h
 * Author: pavel
 *
 * Created on 16. září 2014, 16:10
 */

#ifndef MSGINDELCONDITION_H
#define	MSGINDELCONDITION_H

#include "IMsgInLoginRequired.h"


class MsgInDelCondition :public IMsgInLoginRequired{
public:
    MsgInDelCondition(char* msg, pugi::xml_document* doc);
    virtual ~MsgInDelCondition();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();

    static const string state;
private:

};

#endif	/* MSGINDELCONDITION_H */

