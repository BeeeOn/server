/* 
 * File:   MsgInReinit.cpp
 * Author: pavel
 * 
 * Created on 22. červenec 2014, 10:40
 */

#include "MsgInReinit.h"

MsgInReinit::MsgInReinit(char* msg, pugi::xml_document* doc) : IMsgInLoginAndAdapterAccessRequired(msg, doc) {
    
}

MsgInReinit::~MsgInReinit() {
}

