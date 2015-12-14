#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ServerException.h"

using namespace std;

    const int ServerException::OK = 0;
    const int ServerException::VERSION = 1;
    const int ServerException::TOKEN_EMAIL = 2;
    const int ServerException::EMAIL = 3;
    const int ServerException::USER_ID = 3;

    const int ServerException::IDENTIFICATION_TAKEN = 3;
    const int ServerException::LOCALE = 4;

    const int ServerException::GATEWAY_ID = 5;
    const int ServerException::GATEWAY_TAKEN = 6;

    const int ServerException::AGG_FUNCTION = 8;
    const int ServerException::INTERVAL = 9;
    const int ServerException::DEVICE_ID_TYPE = 10;

    const int ServerException::XML = 12;

    const int ServerException::MISSING_ENTITY = 13;
    const int ServerException::ICON = 14;
    const int ServerException::ACTION = 15;
    const int ServerException::MSG_RIGHT = 16;
    const int ServerException::ROLE = 17;
    const int ServerException::EMAIL_ROLE = 17;
    const int ServerException::TIMEZONE_UTC = 18;

    const int ServerException::BAD_ACTOR_VALUE = 19;

    const int ServerException::RESIGN = 20;
    const int ServerException::NOTREG_A = 21;
    const int ServerException::NOTREG_B = 22;

    const int ServerException::USER_DONOT_EXISTS=24;

    const int ServerException::WRONG_AUTH_PROVIDER = 27;
    const int ServerException::NO_MAIL_PROVIDED = 28;

    const int ServerException::EMAIL_ALREADY_TAKEN = 29;
    const int ServerException::LOGOUT_ERROR = 30;
    const int ServerException::GATEWAY_ACCESSIBLE = 31;

    const int ServerException::ROOM_TYPE = 50;
    const int ServerException::NOTIFICATION_ID = 996;

    const int ServerException::SERVER2SERVER = 100;
    const int ServerException::ADA_SERVER_TIMEOUT = 101;
    const int ServerException::GATEWAY_ACTION_FAIL = 102;  
    

    const int ServerException::UI2ALGO = 200;

    const int ServerException::WRONG_COND_OR_EMTPY_COND = 200;
    const int ServerException::WRONG_OR_EMPTY_ACTION = 201;
    const int ServerException::CONDITION_ACTION = 202;

    const int ServerException::GCMID = 300;
  
    const int ServerException::RANDOMGEN = 997;
    const int ServerException::UNKNOWN_ERROR = 998;
    const int ServerException::MSG_UNKNOWN = 999;
    
    
    const int ServerException::FORBIDDEN_ACTION = 1001;

ServerException::ServerException(int errCode) 
    :   runtime_error("ui_server exc"), _errCode(errCode), _errText("") {
}

ServerException::ServerException(int errCode, string errText) 
    :   runtime_error("ui_server exc"), _errCode(errCode), _errText(errText)  
{

}

const char* ServerException::what() const throw()
{    
    return "Server exception";
}

int ServerException::getErrCode()
{
    return _errCode;
}

string ServerException::getErrText()
{
    return _errText;
} 