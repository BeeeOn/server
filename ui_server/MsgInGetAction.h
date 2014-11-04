/* 
 * File:   MsgInGetAction.h
 * Author: pavel
 *
 * Created on 17. září 2014, 14:44
 */

#ifndef MSGINGETACTION_H
#define	MSGINGETACTION_H

#include "IMsgInLoginRequired.h"


class MsgInGetAction : public IMsgInLoginRequired{
public:
    MsgInGetAction(char* msg, pugi::xml_document* doc);
    virtual ~MsgInGetAction();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();
    static const string state;

private:

};

#endif	/* MSGINGETACTION_H */

