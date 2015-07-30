/* 
 * File:   IMsgInLoginAndAdapterAccessRequired.cpp
 * Author: pavel
 * 
 * Created on 21. červenec 2014, 11:58
 */

#include "IMsgInLoginAndAdapterAccessRequired.h"
#include "../DAO/DAOUsers.h"

using namespace std;

IMsgInLoginAndAdapterAccessRequired::IMsgInLoginAndAdapterAccessRequired(pugi::xml_document* doc) : IMsgInLoginRequired(doc) {
    
    _adapterId = _doc->child(P_COMMUNICATION).attribute(P_ADAPTER_ID).value();
    
    _gatewayId = _doc->child(P_COMMUNICATION).attribute(P_ADAPTER_ID).as_llong(-1);

}


IMsgInLoginAndAdapterAccessRequired::~IMsgInLoginAndAdapterAccessRequired() {
}

enumAccessStatus IMsgInLoginAndAdapterAccessRequired::checkAccess(){
    if( !isComIdValid() )
        return FORBIDDEN_NOT_LOGGED;
    
    //TODO přístup do paměti bez try catch, ale je to mimo kontruktor, tak mozna to je OK
    std::string role = DAOUsers::getInstance().getUserRoleM(_userId, _gatewayId);
    
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
         Logger::getInstance(Logger::ERROR) << "undefined role:>"<<role <<"< ";
    }
    Logger::getInstance(Logger::DEBUG3) << "check role: "<< _userId <<" on "<<_state<<"("<<role<<"="<<roleId<<" "<<_state<<"="<<this->getMsgAuthorization() <<")"<<endl;

    _role = role;
    
    if( roleId >= this->getMsgAuthorization()  ){
         Logger::getInstance(Logger::DEBUG3) << "msg right OK "<<endl;
         return GRANTED;
    }else{
        Logger::getInstance(Logger::ERROR) << " NOT OK "<< _userId <<" on "<<_state<<"("<<role<<"="<<roleId<<" "<<_state<<"="<<this->getMsgAuthorization() <<") "<<endl;
        return FORBIDDEN_WRONG_RIGHTS;
    }
}



string IMsgInLoginAndAdapterAccessRequired::envelopeResponseWithAdapterId(string state, string response)
{        
    string additionalAttributes;
    additionalAttributes = (string)P_ADAPTER_ID+"=\"" + _adapterId +"\"" ;
    return envelopeResponseSetAttributes( state, response, additionalAttributes);
}

string IMsgInLoginAndAdapterAccessRequired::envelopeResponseWithAdapterId(string state, string response, string adapterId)
{        
    string additionalAttributes = "";
    if(adapterId != "")
        additionalAttributes = (string)P_ADAPTER_ID+"=\"" + adapterId +"\"" ;
    return envelopeResponseSetAttributes( state, response, additionalAttributes);
}