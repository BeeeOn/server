/* 
 * File:   MsgInUpdateCondition.h
 * Author: pavel
 *
 * Created on 16. září 2014, 15:47
 */

#ifndef MSGINUPDATECONDITION_H
#define	MSGINUPDATECONDITION_H

#include "IMsgInLoginRequired.h"


class MsgInUpdateCondition : public IMsgInLoginRequired {
public:
    MsgInUpdateCondition(char* msg, pugi::xml_document* doc);
    virtual ~MsgInUpdateCondition();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();
    static const string state;

private:

};

#endif	/* MSGINUPDATECONDITION_H */

