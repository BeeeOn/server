/* 
 * File:   testHelper.h
 * Author: pavel
 *
 * Created on 22. červenec 2014, 14:37
 */

#ifndef TESTHELPER_H
#define	TESTHELPER_H

#define TESTING

#include "DBConnector.h"
#include "communication.h"
#include "pugiconfig.hpp"
#include "pugixml.hpp"
/*
 const int ServerException::VERSION;
 const int ServerException::TOKEN_EMAIL;
 const int ServerException::LOCALE;
 const int ServerException::ADAPTER_ID;
 const int ServerException::AGG_FUNCTION;
 const int ServerException::INTERVAL;
 const int ServerException::DEVICE_ID_TYPE; 
 const int ServerException::ROOM_TYPE;
 const int ServerException::XML;
 const int ServerException::MISSING_ENTITY;
 const int ServerException::ICON;
 const int ServerException::ACTION;
 const int ServerException::MSG_RIGHT;
 const int ServerException::EMAIL_ROLE; 
 const int ServerException::TIME_UTC;
 const int ServerException::MSG_UNKNOWN ;
 const int ServerException::NOTREG_A;
 */
#include "ServerException.h"
#define COM_VERSION "\"2.2\""
#define TESTDB1_CON_STRING "user='pavel' dbname = 'home3' password='1234'"
#define NOT_IMPLEMENTED "not implemented"

string getState(pugi::xml_document* doc);
int getSessionId(pugi::xml_document* doc);
int getErrCode(pugi::xml_document* doc);

void createMsgInWithAttributes(char*   msgin, string com_ver,string ses_id,string state);
void createMsgInWithAttributes(char*   msgin, string com_ver,string ses_id,string state,string attr1name, string attr1value);
void createMsgInWithAttributes(char*   msgin, string com_ver,string ses_id,string state,string attr1name, string attr1value,string attr2name, string attr2value,string attr3name, string attr3value);
void createMsgInWithAttributes( char*  msgin, string com_ver,string ses_id,string state,string attr1name, string attr1value,string attr2name, string attr2value,string attr3name, string attr3value,string attr4name, string attr4value);
void createMsgInWithAttributes( char*  msgin, string com_ver,string ses_id,string state,string attr1name, string attr1value,string attr2name, string attr2value,string attr3name, string attr3value,string attr4name, string attr4value,
                                                   string attr5name,string attr5value,string attr6name, string attr6value,string attr7name, string attr7value );
#endif	/* TESTHELPER_H */
    
