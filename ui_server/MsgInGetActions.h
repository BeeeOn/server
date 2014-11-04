/* 
 * File:   MsgInGetActions.h
 * Author: pavel
 *
 * Created on 17. září 2014, 13:41
 */

#ifndef MSGINGETACTIONS_H
#define	MSGINGETACTIONS_H

#include "IMsgInLoginRequired.h"


class MsgInGetActions : public IMsgInLoginRequired{
public:
    MsgInGetActions(char* msg, pugi::xml_document* doc);
    virtual ~MsgInGetActions();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();
    static const string state;

private:

};

#endif	/* MSGINGETACTIONS_H */

