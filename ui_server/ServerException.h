/* 
 * File:   ServerException.h
 * Author: pavel
 *
 * Created on 17. červenec 2014, 13:46
 */

#ifndef SERVEREXCEPTION_H
#define	SERVEREXCEPTION_H

#include <exception>
#include <stdexcept>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
class ServerException : public runtime_error {
public:
    ServerException(int errCode);
    ServerException(int errCode, string errText);
   // ServerException(int errCode, string errText, string id, string state);
   // virtual ~ServerException() throw();
    virtual ~ServerException() throw() {};
    virtual const char* what() const throw();
private:
    int _errCode;
    string _errText;
    /*string _id;
    string _state;*/
public:
    int getErrCode();
    string getErrText();
   /* string getId();
    string getState();*/
public:
    static const int VERSION = 1;
    static const int TOKEN_EMAIL = 2;
    static const int EMAIL = 3; //+text
    static const int LOCALE = 4;

    static const int ADAPTER_ID = 5;

    static const int AGG_FUNCTION = 8;
    static const int INTERVAL = 9;
    static const int DEVICE_ID_TYPE = 10; //+text

    static const int XML = 12;

    static const int MISSING_ENTITY = 13;
    static const int ICON = 14;
    static const int ACTION = 15;
    static const int MSG_RIGHT = 16;
    static const int ROLE = 17; //+text
    static const int EMAIL_ROLE = 17; //+text
    static const int TIME_UTC = 18;

    static const int BAD_ACTOR_VALUE = 19;

    static const int RESIGN = 20;
    static const int NOTREG_A = 21;
    static const int NOTREG_B = 22;

    static const int ROOM_TYPE = 50;

    static const int SERVER2SERVER = 100;
    static const int SWITCH_FAIL = 100;
    static const int ADAPTER_LISTEN_FAIL =100;

    static const int WRONG_COND_OR_EMTPY_COND = 200;
    static const int WRONG_OR_EMPTY_ACTION = 201;
    static const int CONDITION_ACTION = 202;

    static const int UNKNOWN_ERROR =998;
    
    static const int MSG_UNKNOWN = 999;
    
};
#endif	/* SERVEREXCEPTION_H */

