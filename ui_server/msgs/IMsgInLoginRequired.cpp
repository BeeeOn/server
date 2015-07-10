/* 
 * File:   IMsgInLoginRequired.cpp
 * Author: pavel
 * 
 * Created on 21. červenec 2014, 10:57
 */

#include "IMsgInLoginRequired.h"
#include "../DAO/DAOUsers.h"
#include "IMsgInLoginUnwanted.h"

using namespace std;

IMsgInLoginRequired::IMsgInLoginRequired(pugi::xml_document* doc): IMsgInFreeAccess(doc) {
    _userId = -1;
}

IMsgInLoginRequired::~IMsgInLoginRequired() {
}

bool IMsgInLoginRequired::isComIdValid(){
    
    User user = DAOUsers::getInstance().getUserAssociatedWithToken(_token);
    
    _userId = user.user_id;
    _requesterMail = user.mail;
    
    Logger::getInstance(Logger::DEBUG)<<"user:"<<_userId<<" token:"<<_token<<endl;
    
    return (_userId > 0 );
}

enumAccessStatus IMsgInLoginRequired::checkAccess(){
    if( isComIdValid() )
        return GRANTED;
    else
        return FORBIDDEN_NOT_LOGGED;
}
