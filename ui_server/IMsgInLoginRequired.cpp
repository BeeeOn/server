/* 
 * File:   IMsgInLoginRequired.cpp
 * Author: pavel
 * 
 * Created on 21. červenec 2014, 10:57
 */

#include "IMsgInLoginRequired.h"
#include "DBConnector.h"

IMsgInLoginRequired::IMsgInLoginRequired(char* msg, pugi::xml_document* doc): IMsgIn(msg, doc) {
    
    Logger::getInstance(Logger::DEBUG)<<"user:"<<_gUserId<<" comId:"<<_IHAtoken<<endl;
}

IMsgInLoginRequired::~IMsgInLoginRequired() {
}

bool IMsgInLoginRequired::isComIdValid(){
    _gUserId = DBConnector::getInstance().getUserIdbyIhaToken(_IHAtoken);
    return (_gUserId != "" );
}

enumAccessStatus IMsgInLoginRequired::checkAccess(){
    if( isComIdValid() )
        return GRANTED;
    else
        return FORBIDDEN_NOT_LOGGED;
}
