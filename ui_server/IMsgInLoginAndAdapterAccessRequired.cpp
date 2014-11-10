/* 
 * File:   IMsgInLoginAndAdapterAccessRequired.cpp
 * Author: pavel
 * 
 * Created on 21. červenec 2014, 11:58
 */

#include "IMsgInLoginAndAdapterAccessRequired.h"
#include "DBConnector.h"



IMsgInLoginAndAdapterAccessRequired::IMsgInLoginAndAdapterAccessRequired(char* msg, pugi::xml_document* doc) : IMsgInLoginRequired(msg, doc) {
}


IMsgInLoginAndAdapterAccessRequired::~IMsgInLoginAndAdapterAccessRequired() {
}

enumAccessStatus IMsgInLoginAndAdapterAccessRequired::checkAccess(){
    
    if(_parredUserMail == "")
        return FORBIDDEN_NOT_LOGGED;
    
    string role = DBConnector::getInstance().getUserRole(_parredUserMail, _adapterId);
    
    int roleId;
    if(role == P_ROLE_GUEST)
        roleId= GUEST;
    else if(role == P_ROLE_USER)
        roleId= USER;
    else if(role == P_ROLE_ADMIN)
        roleId= ADMIN;
    else if(role == P_ROLE_SUPERUSER)
        roleId= SUPERUSER;
    else {
        roleId = -1;
         Logger::getInstance(Logger::DEBUG3) << "undefined role:>"<<role <<"< ";
    }
    Logger::getInstance(Logger::DEBUG3) << "check role: "<< _parredUserMail <<" on "<<_state<<"("<<role<<"="<<roleId<<" "<<_state<<"="<<this->getMsgAuthorization() <<")"<<endl;
    
    if(roleId == -1){//this is handled by checker in msgFactory
        Logger::getInstance(Logger::DEBUG3) << "wrong msg: "<<_state<<endl;
        return FORBIDDEN_WRONG_RIGHTS;
    }
    _role = role;
    
    if( roleId >= this->getMsgAuthorization()  ){
         Logger::getInstance(Logger::DEBUG3) << "msg right OK "<<endl;
         return GRANTED;
    }else{
        Logger::getInstance(Logger::ERROR) << " NOT OK "<< _parredUserMail <<" on "<<_state<<"("<<role<<"="<<roleId<<" "<<_state<<"="<<this->getMsgAuthorization() <<") "<<endl;
        return FORBIDDEN_WRONG_RIGHTS;
    }
    
    
}