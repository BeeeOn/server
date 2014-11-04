/* 
 * File:   MsgInReinit.h
 * Author: pavel
 *
 * Created on 22. červenec 2014, 10:40
 */

#ifndef MSGINREINIT_H
#define	MSGINREINIT_H

#include "IMsgInLoginAndAdapterAccessRequired.h"


class MsgInReinit : public IMsgInLoginAndAdapterAccessRequired{
public:
    MsgInReinit(char* msg, pugi::xml_document* doc);
    virtual ~MsgInReinit();
private:
    string _oldAdapter;
    string _newAdapter;
};

#endif	/* MSGINREINIT_H */

