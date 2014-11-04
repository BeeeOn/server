/* 
 * File:   IMsgInLoginRequired.cpp
 * Author: pavel
 * 
 * Created on 21. červenec 2014, 10:57
 */

#include "IMsgInLoginRequired.h"

IMsgInLoginRequired::IMsgInLoginRequired(char* msg, pugi::xml_document* doc): IMsgIn(msg, doc) {
    
    if(_comId != 0){
        user u = ComTable::getInstance().getUserByComId(_comId);
        _parredUserMail = u.mail;
        _parredUserId = u.id;
    }else{
        _parredUserMail = "";
    }
    Logger::getInstance(Logger::DEBUG)<<"user:"<<_parredUserMail<<" comId:"<<_comId<<endl;
}

IMsgInLoginRequired::~IMsgInLoginRequired() {
}

bool IMsgInLoginRequired::isComIdValid(){
    return ( _parredUserMail.compare("") != 0);
}

enumAccessStatus IMsgInLoginRequired::checkAccess(){
    if( isComIdValid() )
        return GRANTED;
    else
        return FORBIDDEN_NOT_LOGGED;
}
