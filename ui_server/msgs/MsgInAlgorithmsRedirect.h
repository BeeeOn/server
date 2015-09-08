/* 
 * File:   MsgInAlgorithmsRedirect.h
 * Author: pavel
 *
 * Created on 16. březen 2015, 14:50
 */

#ifndef MSGINALGORITHMSREDIRECT_H
#define	MSGINALGORITHMSREDIRECT_H

#include "IMsgInLoginRequired.h"


class MsgInAlgorithmsRedirect : public IMsgInLoginRequired{
public:
    MsgInAlgorithmsRedirect(pugi::xml_document* doc);
    virtual ~MsgInAlgorithmsRedirect();
    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();
    static const std::string state;

};

#endif	/* MSGINALGORITHMSREDIRECT_H */

