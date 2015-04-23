/* 
 * File:   MsgInGamificationRedirect.h
 * Author: Pavel
 *
 * Created on 22. dubna 2015, 10:12
 */

#ifndef MSGINGAMIFICATIONREDIRECT_H
#define	MSGINGAMIFICATIONREDIRECT_H

#include "IMsgInLoginRequired.h"

class MsgInGamificationRedirect : public IMsgInLoginRequired {
public:
    MsgInGamificationRedirect(char* msg, pugi::xml_document* doc);
    virtual ~MsgInGamificationRedirect();
    virtual int getMsgAuthorization();

    virtual std::string createResponseMsgOut();
    static const std::string state;

};

#endif	/* MSGINGAMIFICATIONREDIRECT_H */

