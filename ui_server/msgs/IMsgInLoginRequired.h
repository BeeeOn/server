/* 
 * File:   IMsgInLoginRequired.h
 * Author: pavel
 *
 * Created on 21. červenec 2014, 10:57
 */

#ifndef IMSGINLOGINREQUIRED_H
#define	IMSGINLOGINREQUIRED_H

#include "IMsgIn.h"
#include "IMsgInLoginUnwanted.h"

class IMsgInLoginRequired : public IMsgInFreeAccess{
public:
    IMsgInLoginRequired(pugi::xml_document* doc);
    virtual ~IMsgInLoginRequired();
    virtual bool isComIdValid();
    virtual enumAccessStatus checkAccess();

    int _userId;
    std::string _requesterMail;
};

#endif	/* IMSGINLOGINREQUIRED_H */

