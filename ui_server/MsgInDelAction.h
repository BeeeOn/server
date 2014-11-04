/* 
 * File:   MsgInDelAction.h
 * Author: pavel
 *
 * Created on 17. září 2014, 15:54
 */

#ifndef MSGINDELACTION_H
#define	MSGINDELACTION_H

#include "IMsgInLoginRequired.h"


class MsgInDelAction : public IMsgInLoginRequired {
public:
    MsgInDelAction(char* msg, pugi::xml_document* doc);
    virtual ~MsgInDelAction();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();
    static const string state;

private:

};

#endif	/* MSGINDELACTION_H */

