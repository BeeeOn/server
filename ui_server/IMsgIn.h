#ifndef IMSGIN_H
#define	IMSGIN_H



#define P_COMMUNICATION "com"
#define P_VERSION "ver"
#define P_ERRCODE "errcode"
#define P_STATE "state"
#define P_EMAIL "email"
#define P_SESSION_ID "uid"
#define P_GOOGLE_ID "gid"

#define P_ADAPTER "adapter"
#define P_FACILITY "dev"
#define P_DEVICE "part"

#define P_ADAPTER_ID "aid"
#define P_IN_ADAPTER_ID "id"
#define P_ADAPTER_NAME "aname"

#define P_FACILITY_ID "did"
#define P_IN_FACILITY_ID "id"
#define P_FACILITY_INIT "init"
#define P_FACILITY_REFRESH "refresh"
#define P_FACILITY_LOCATION_ID "lid"
#define P_FACILITY_BATTERY "batt"
#define P_FACILITY_QUALITY "rssi"

#define P_DEVICE_ID "did"
#define P_IN_DEVICE_TYPE "type"
#define P_DEVICE_TYPE "dtype"
#define P_DEVICE_VALUE "val"
#define P_DEVICE_VISIBILITY "vis"
#define P_DEVICE_NAME "name"

#define P_GOOGLE_TOKEN "gt"
#define P_LOCALIZATION "loc"
#define P_GOOGLE_CLOUD_MID "gcmid"

#define P_LOG_FUNC_TYPE "ftype"
#define P_LOG_INTERVAL "interval"
#define P_LOG_FROM "from"
#define P_LOG_TO "to"

#define P_ROLE_GUEST "guest"
#define P_ROLE_USER "user"
#define P_ROLE_ADMIN "admin"
#define P_ROLE_SUPERUSER "superuser"

#define P_USER "user"
#define P_ROLE "role"

#define P_ROOM "loc"
#define P_ROOM_ID "lid"
#define P_ROOM_NAME "lname"
#define P_ROOM_TYPE "ltype"

#define P_IN_ROOM_ID "id"
#define P_IN_ROOM_NAME "name"
#define P_IN_ROOM_TYPE "type"
#define P_ROOM_MIN_TYPE 1
#define P_ROOM_MAX_TYPE 6

#define P_TIME_UTC "utc"
#define P_TIME_MIN -1000
#define P_TIME_MAX 1000

#define P_PHONE_ID "pid"

/******************************************/

#define R_TRUE "true"
#define R_FALSE "false"
#define R_UID "uid"
#define R_ACCOUNTS "accounts"
#define R_DEVICES_ALL "alldevs"
#define R_DEVICES "devs"
#define R_ADAPTERS "adapters"
#define R_TIMEZONE "timezone"
#define R_ROOM_CREATED "roomid"
#define R_ROOMS "rooms"
#define R_VIEWS "views"
#define R_LOG "logdata"

#define EVERYONE 1
#define GUEST 2
#define USER 3
#define ADMIN 4
#define SUPERUSER 5

#include <string>
#include <iostream>
#include <inttypes.h>
        
#include "ComTable.h"

#include "pugiconfig.hpp"
#include "pugixml.hpp"

#include "Logger.h"
#include "ServerException.h"


using namespace std;


struct device {
    string id;
    string type;
};

typedef long long int IhaToken;
typedef string GUserId;

enum enumAccessStatus { GRANTED, FORBIDDEN_NOT_LOGGED, FORBIDDEN_WRONG_RIGHTS };

class IMsgIn
{
public:
        IMsgIn(char* msg, pugi::xml_document* doc);
        virtual ~IMsgIn(void);
        virtual string createResponseMsgOut() = 0;
        
        virtual enumAccessStatus checkAccess() = 0;
        virtual int getMsgAuthorization() = 0;
        virtual bool isComIdValid() = 0;
        bool checkProtocolVersion();
public:
        std::string _state;
        long long int _IHAtoken;
        string _adapterId;
protected:
        char* _msg;
        pugi::xml_document* _doc;
        string _msgOut;
        
        
public:
    string envelopeResponse(string state);
    string envelopeResponse(string state, string response);
    string envelopeResponse(string state, string response, int errcode);
    string envelopeResponseWithRole(string state, string response, string role);
    string envelopeResponseWithAdapterId(string state, string response);
    string envelopeResponseWithAdapterId(string state, string response, string adapterId);
    string envelopeResponseWithAttributes(string state, string attributes);
  /*  int _comId;
    string _additionalInfo;
    string _requestState;
    */
    static const string VERSION;
protected:
    string makeXMLattribute(string attr,string value);
private:
    string envelopeResponseSetAttributes(string state, string response, string attributes);
        
};

#endif /* IMSGIN_H */
