/* 
 * File:   MsgInUpdateAction.h
 * Author: pavel
 *
 * Created on 17. září 2014, 15:28
 */

#ifndef MSGINUPDATEACTION_H
#define	MSGINUPDATEACTION_H

#include "IMsgInLoginRequired.h"


class MsgInUpdateAction : public IMsgInLoginRequired {
public:
    MsgInUpdateAction(char* msg, pugi::xml_document* doc);
    virtual ~MsgInUpdateAction();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();
    static const string state;

private:

};

#endif	/* MSGINUPDATEACTION_H */

