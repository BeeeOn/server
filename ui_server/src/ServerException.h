#ifndef SERVEREXCEPTION_H
#define	SERVEREXCEPTION_H

#include <exception>
#include <stdexcept>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

class ServerException : public std::runtime_error {
public:
    ServerException(int errCode);
    ServerException(int errCode, std::string errText);
    virtual ~ServerException() throw() {};
    virtual const char* what() const throw();
private:
    int _errCode;
    std::string _errText;
public:
    int getErrCode();
    std::string getErrText();
public:
    static const int OK;
    static const int VERSION;
    static const int TOKEN_EMAIL;
    static const int EMAIL;
    static const int USER_ID;
    static const int IDENTIFICATION_TAKEN;
    static const int LOCALE;

    static const int GATEWAY_ID;
    static const int GATEWAY_TAKEN;
    static const int GATEWAY_ACCESSIBLE;

    static const int AGG_FUNCTION;
    static const int INTERVAL;
    static const int DEVICE_ID_TYPE; //+text

    static const int XML;

    static const int MISSING_ENTITY;
    static const int ICON;
    static const int ACTION;
    static const int MSG_RIGHT;
    static const int ROLE; //+text
    static const int EMAIL_ROLE; //+text
    static const int TIMEZONE_UTC;

    static const int BAD_ACTOR_VALUE;

    static const int RESIGN;
    static const int NOTREG_A;
    static const int NOTREG_B;
    
    static  const int USER_DONOT_EXISTS;
    static  const int WRONG_AUTH_PROVIDER;
    static  const int NO_MAIL_PROVIDED;
    static  const int EMAIL_ALREADY_TAKEN;
    static  const int LOGOUT_ERROR;

    static const int ROOM_TYPE;
    static const int NOTIFICATION_ID;

    static const int SERVER2SERVER;
    static const int GATEWAY_ACTION_FAIL;
    static const int ADA_SERVER_TIMEOUT;
    
    static const int UI2ALGO;

    static const int WRONG_COND_OR_EMTPY_COND;
    static const int WRONG_OR_EMPTY_ACTION;
    static const int CONDITION_ACTION;

    static const int GCMID;
    
    
    static const int UNKNOWN_ERROR;
    static const int RANDOMGEN;
    static const int MSG_UNKNOWN;
    
    static const int FORBIDDEN_ACTION;
    
};
#endif	/* SERVEREXCEPTION_H */

