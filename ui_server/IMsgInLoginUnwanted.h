/* 
 * File:   IMsgInLoginUnwanted.h
 * Author: pavel
 *
 * Created on 21. červenec 2014, 11:05
 */

#ifndef IMSGINLOGINUNWANTED_H
#define	IMSGINLOGINUNWANTED_H

#include "IMsgIn.h"


class IMsgInFreeAccess : public IMsgIn{
public:
    IMsgInFreeAccess(char* msg, pugi::xml_document* doc);
    virtual ~IMsgInFreeAccess();
    
    virtual int getMsgAuthorization();
    virtual bool isComIdValid();

    virtual enumAccessStatus checkAccess();

private:

};

#endif	/* IMSGINLOGINUNWANTED_H */

