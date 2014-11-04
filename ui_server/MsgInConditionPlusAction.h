/* 
 * File:   MsgInConditionPlusAction.h
 * Author: pavel
 *
 * Created on 17. září 2014, 16:08
 */

#ifndef MSGINCONDITIONPLUSACTION_H
#define	MSGINCONDITIONPLUSACTION_H

#include "IMsgInLoginRequired.h"


class MsgInConditionPlusAction: public IMsgInLoginRequired {
public:
    MsgInConditionPlusAction(char* msg, pugi::xml_document* doc) ;
    virtual ~MsgInConditionPlusAction();

    virtual int getMsgAuthorization();

    virtual string createResponseMsgOut();
    static const string state;


private:

};

#endif	/* MSGINCONDITIONPLUSACTION_H */

