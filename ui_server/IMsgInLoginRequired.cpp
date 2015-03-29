/* 
 * File:   IMsgInLoginRequired.cpp
 * Author: pavel
 * 
 * Created on 21. červenec 2014, 10:57
 */

#include "IMsgInLoginRequired.h"
#include "../DAO/DAOUsers.h"

IMsgInLoginRequired::IMsgInLoginRequired(char* msg, pugi::xml_document* doc): IMsgIn(msg, doc) {
    
    Logger::getInstance(Logger::DEBUG)<<"user:"<<_userId<<" comId:"<<_token<<endl;
}

IMsgInLoginRequired::~IMsgInLoginRequired() {
}

bool IMsgInLoginRequired::isComIdValid(){
    _userId = DAOUsers::getInstance().getUserIdbyIhaToken(_token);
    return (_userId > 0 );
}

enumAccessStatus IMsgInLoginRequired::checkAccess(){
    if( isComIdValid() )
        return GRANTED;
    else
        return FORBIDDEN_NOT_LOGGED;
}
