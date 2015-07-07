/* 
 * File:   IMsgInLoginUnwanted.cpp
 * Author: pavel
 * 
 * Created on 21. červenec 2014, 11:05
 */

#include "IMsgInLoginUnwanted.h"

IMsgInFreeAccess::IMsgInFreeAccess(pugi::xml_document* doc): IMsgIn(doc) {
}


IMsgInFreeAccess::~IMsgInFreeAccess() {
}

bool IMsgInFreeAccess::isComIdValid(){
    return true;
}

enumAccessStatus IMsgInFreeAccess::checkAccess(){
    return GRANTED;
}

int IMsgInFreeAccess::getMsgAuthorization() {
    return EVERYONE;
}