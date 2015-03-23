/* 
 * File:   MsgRightsChecker.cpp
 * Author: pavel
 * 
 * Created on 3. červenec 2014, 12:47
 */

#include "MsgRightsChecker.h"

#include "MsgInSignMe.h"
#include "MsgInGetAdapters.h"
#include "MsgInGetDevices.h"
#include "MsgInGetLog.h"
#include "MsgInPartial.h"
#include "MsgInAddRoom.h"
#include "MsgInGetAllDevices.h"
#include "MsgInUpdateRooms.h"
#include "MsgInDelRoom.h"
#include "MsgInGetRooms.h"
#include "MsgInAddView.h"
#include "MsgInUpdateView.h"
#include "MsgInDelView.h"
#include "MsgInGetViews.h"
#include "MsgInAddAccount.h"
#include "MsgInChangeAccount.h"
#include "MsgInDelAccount.h"
#include "MsgInGetAccount.h"
#include "MsgInSetTimeZone.h"
#include "MsgInGetTimeZone.h"
#include "IMsgIn.h"
#include <stdio.h>
/*******************************************
 Role
 Název	 Práva
 -------------------
 guest	 číst adaptér a data senzorů
 user	 guest + přepínat aktory
 admin	 user + změna nastavení senzorů (přejmenování, refresh, logování, ...)
 superuser	 admin + spravovat jiné účty (přidávat/odebírat uživatele, nastavovat role), první a poslední učet
 */
#define EVERYONE 1
#define GUEST 2
#define USER 3
#define ADMIN 4
#define SUPERUSER 5


using namespace std;

MsgRightsChecker::MsgRightsChecker() {
    this->_rightsTable["guest"] = GUEST;
    this->_rightsTable["user"] = USER;
    this->_rightsTable["admin"] = ADMIN;
    this->_rightsTable["superuser"] = SUPERUSER;
    
    /*MSGS*/
    this->_msgRightsTable[MsgInSignMe::state] = EVERYONE;
    this->_msgRightsTable[MsgInGetAdapters::state] = GUEST;
    /*SENSORS*/
    this->_msgRightsTable[MsgInGetAllDevices::state] = GUEST; 
    this->_msgRightsTable[MsgInGetDevs::state] = GUEST;
    this->_msgRightsTable[MsgInLogName::state] = GUEST;
    this->_msgRightsTable[MsgInDevices::state] = ADMIN; // USER na prepinani aktoru
    /*ROOMS*/
    this->_msgRightsTable[MsgInAddRoom::state] = ADMIN;
    this->_msgRightsTable[MsgInUpdateRooms::state] = ADMIN;
    this->_msgRightsTable[MsgInDelRoom::state] = ADMIN;
    this->_msgRightsTable[MsgInGetRooms::state] = GUEST;
    /*VIEWS*/
    this->_msgRightsTable[MsgInAddView::state] = GUEST;
    this->_msgRightsTable[MsgInUpdateView::state] = GUEST;
    this->_msgRightsTable[MsgInDelView::state] = GUEST;
    this->_msgRightsTable[MsgInGetViews::state] = GUEST;
    /*ACCOUNTS*/
    this->_msgRightsTable[MsgInAddAccount::state] = SUPERUSER;
    this->_msgRightsTable[MsgInChangeConAccount::state] = SUPERUSER;
    this->_msgRightsTable[MsgInDelConAccount::state] = SUPERUSER;
    this->_msgRightsTable[MsgInGetConAccount::state] = SUPERUSER;
    /*TIME*/
    this->_msgRightsTable[MsgInSetTimeZone::state] = ADMIN;
    this->_msgRightsTable[MsgInGetTimeZone::state] = GUEST;
    
}

MsgRightsChecker::~MsgRightsChecker() {
}

MsgRightsChecker& MsgRightsChecker::getInstance()
{
    static MsgRightsChecker instance;
    return instance;
}


bool MsgRightsChecker::checkRights(IMsgInLoginAndAdapterAccessRequired& msg){
   string role = DBConnector::getInstance().getUserRoleM(msg._gUserId, msg._adapterId);
    Logger::getInstance(Logger::DEBUG) << "check role: "<< msg._gUserId <<" on "<<msg._state<<"("<<role<<"="<<_rightsTable[role] <<" "<<msg._state<<"="<<_msgRightsTable[msg._state] <<") \n";
    
    if(_msgRightsTable[msg._state] == 0){//this is handled by checker in msgFactory
        Logger::getInstance(Logger::DEBUG) << "wrong msg: "<<msg._state<<"\n";
        return false;
    }
    msg._role = role;
    
    if( _rightsTable[role] >= _msgRightsTable[msg._state] ){
         Logger::getInstance() << "msg right OK \n";
         return true;
    }else{
        Logger::getInstance() << " NOT OK \n";
        return false;
    }
}