/* 
 * File:   MsgInSetAction.h
 * Author: pavel
 *
 * Created on 17. září 2014, 12:37
 */

#ifndef MSGINSETACTION_H
#define	MSGINSETACTION_H

#include "IMsgInLoginRequired.h"


class MsgInSetAction : public IMsgInLoginRequired{
public:
    MsgInSetAction(char* msg, pugi::xml_document* doc);
    virtual ~MsgInSetAction();

    virtual int getMsgAuthorization();
    virtual string createResponseMsgOut();
    static const string state;

private:

};

#endif	/* MSGINSETACTION_H */

