/* 
 * File:   MsgInReinit.cpp
 * Author: pavel
 * 
 * Created on 22. červenec 2014, 10:40
 */

#include "MsgInReinitAdapter.h"

MsgInReinitAdapter::MsgInReinitAdapter(char* msg, pugi::xml_document* doc) : IMsgInLoginAndAdapterAccessRequired(msg, doc) {
    
}

MsgInReinitAdapter::~MsgInReinitAdapter() {
}

//TODO dodělat reinit