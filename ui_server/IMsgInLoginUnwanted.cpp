/* 
 * File:   IMsgInLoginUnwanted.cpp
 * Author: pavel
 * 
 * Created on 21. červenec 2014, 11:05
 */

#include "IMsgInLoginUnwanted.h"

IMsgInLoginUnwanted::IMsgInLoginUnwanted(char* msg, pugi::xml_document* doc): IMsgIn(msg, doc) {
}


IMsgInLoginUnwanted::~IMsgInLoginUnwanted() {
}

bool IMsgInLoginUnwanted::isComIdValid(){
    return true;
}

enumAccessStatus IMsgInLoginUnwanted::checkAccess(){
    return GRANTED;
}

int IMsgInLoginUnwanted::getMsgAuthorization() {
    return EVERYONE;
}