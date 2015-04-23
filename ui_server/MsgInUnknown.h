/* 
 * File:   MsgInUnknown.h
 * Author: pavel
 *
 * Created on 18. duben 2014, 15:55
 */

#ifndef MSGINUNKNOWN_H
#define	MSGINUNKNOWN_H
#include "IMsgIn.h"
#include "IMsgInLoginUnwanted.h"
class MsgInUnknown :
	public IMsgInLoginUnwanted {
public:
    MsgInUnknown(char* msg, pugi::xml_document* doc);
    virtual ~MsgInUnknown();

    virtual int getMsgAuthorization();
    virtual std::string createResponseMsgOut();

	static const std::string state;
private:

};

#endif	/* MSGINUNKNOWN_H */

