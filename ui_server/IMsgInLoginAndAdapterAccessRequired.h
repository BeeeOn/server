/* 
 * File:   IMsgInLoginAndAdapterAccessRequired.h
 * Author: pavel
 *
 * Created on 21. červenec 2014, 11:58
 */

#ifndef IMSGINLOGINANDADAPTERACCESSREQUIRED_H
#define	IMSGINLOGINANDADAPTERACCESSREQUIRED_H

#include "IMsgInLoginRequired.h"
#include "MsgRightsChecker.h"


class IMsgInLoginAndAdapterAccessRequired : public IMsgInLoginRequired{
public:
    IMsgInLoginAndAdapterAccessRequired(char* msg, pugi::xml_document* doc);
    virtual ~IMsgInLoginAndAdapterAccessRequired();
    
    virtual enumAccessStatus checkAccess();

    string _role;
    
};

#endif	/* IMSGINLOGINANDADAPTERACCESSREQUIRED_H */

