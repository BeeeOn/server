#ifndef IMSGIN_H
#define	IMSGIN_H



#define P_COMMUNICATION "com"
#define P_VERSION "ver"
#define P_ERRCODE "errcode"
#define P_STATE "state"
#define P_EMAIL "email"
#define P_SESSION_ID "bt"
#define P_GOOGLE_ID "gid"

#define P_ADAPTER "adapter"
#define P_FACILITY "dev"
#define P_DEVICE "part"

#define P_ADAPTER_ID "aid"
#define P_IN_ADAPTER_ID "id"
#define P_ADAPTER_NAME "aname"
#define P_ADAPTER_IP "ip"
#define P_ADAPTER_VERSION "aversion"
#define P_ADAPTER_N_USERS "nusers"
#define P_ADAPTER_N_FACILITIES "nfacs"

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
#define P_FACEBOOK_TOKEN "fbt"
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

#define P_USER_ID "uid"
#define P_USER_NAME "name"
#define P_USER_SURNAME "surname"
#define P_USER_GENDER "gender"
#define P_USER_EMAIL "email"
#define P_USER_PICTURE "imgurl"
#define P_USER_ACCOUNTS "accounts"

#define P_SERVICE "srv"
#define P_SERVICE_NAME "name"
#define P_SERVICE_ID "id"
#define P_SERVICE_GOOGLE "google"
#define P_SERVICE_FACEBOOK "facebook"
#define P_SERVICE_BEEEON "beeeon"

#define P_NOTIFICATION "notif"
#define P_NOTIFICATION_ID "mid"

#define P_ROOM "loc"
#define P_ROOM_ID "lid"
#define P_ROOM_NAME "lname"
#define P_ROOM_TYPE "ltype"

#define P_SIGN_PARAMS "par"
#define P_SIGN_SERVICE "srv"

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
#define R_GATEINFO "gateinfo"
#define R_TIMEZONE "timezone"
#define R_ROOM_CREATED "roomid"
#define R_ROOMS "rooms"
#define R_VIEWS "views"
#define R_LOG "logdata"
#define R_BEEEON_TOKEN "bt"
#define R_USER_INFO "userinfo"
#define R_NOTIFICATIONS "notifs"

#define EVERYONE 1
#define GUEST 2
#define USER 3
#define ADMIN 4
#define SUPERUSER 5

#include <string>
#include <iostream>
#include <inttypes.h>
        
#include "../ui_logger/Logger.h"
#include "../lib/pugixml.hpp"
#include "ServerException.h"

struct t_version {
    int majorVersion;
    int minorVersion;
};

enum enumAccessStatus { GRANTED, FORBIDDEN_NOT_LOGGED, FORBIDDEN_WRONG_RIGHTS };

class IMsgIn
{
public:
        IMsgIn(pugi::xml_document* doc);
        virtual ~IMsgIn(void);
        virtual std::string createResponseMsgOut() = 0;
        
        virtual enumAccessStatus checkAccess() = 0;
        virtual int getMsgAuthorization() = 0;
        virtual bool isComIdValid() = 0;
        bool checkProtocolVersion();
public:
        std::string _state;
        std::string _token;
        std::string _adapterId;
protected:
        pugi::xml_document* _doc;
        std::string _msgOut;
        
        pugi::xml_document _outputDoc;
        pugi::xml_node _outputMainNode;
        
public:
    std::string envelopeResponse(std::string state);
    std::string envelopeResponse(std::string state, std::string response);
    std::string envelopeResponse(std::string state, std::string response, int errcode);
    std::string envelopeResponseWithRole(std::string state, std::string response, std::string role);
    std::string envelopeResponseWithAdapterId(std::string state, std::string response);
    std::string envelopeResponseWithAdapterId(std::string state, std::string response, std::string adapterId);
    std::string envelopeResponseWithAttributes(std::string state, std::string attributes);
  /*  int _comId;
    string _additionalInfo;
    string _requestState;
    */
    static const int MAJOR_VERSION;
    static const int MINOR_VERSION;
    static const std::string VERSION;
protected:
    std::string makeXMLattribute(std::string attr,std::string value);
    std::string getXMLreply(std::string responseState);
    std::string getNegativeXMLReply(int errorCode);
    std::string getNegativeXMLReply(int errorCode, const char* errorInfo);
    
    bool isRoleValid(std::string role);

private:
    t_version parseProtocolVersion(std::string version);
    std::string envelopeResponseSetAttributes(std::string state, std::string response, std::string attributes);
    
    void makeCommunicationHeader(std::string responseState);
};

#endif /* IMSGIN_H */
