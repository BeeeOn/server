/* 
 * File:   IMsgInLoginRequired.h
 * Author: pavel
 *
 * Created on 21. červenec 2014, 10:57
 */

#ifndef IMSGINLOGINREQUIRED_H
#define	IMSGINLOGINREQUIRED_H

#include "IMsgIn.h"

using namespace std;

class IMsgInLoginRequired : public IMsgIn{
public:
    IMsgInLoginRequired(char* msg, pugi::xml_document* doc);
    virtual ~IMsgInLoginRequired();
    virtual bool isComIdValid();
    virtual enumAccessStatus checkAccess();

    string _parredUserMail;
    int _parredUserId;
};

#endif	/* IMSGINLOGINREQUIRED_H */

