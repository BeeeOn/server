#ifndef IMSGIN_H
#define	IMSGIN_H



#define P_LOCATION_MIN_TYPE 1
#define P_LOCATION_MAX_TYPE 6

#define P_TIME_UTC "utc"
#define P_TIME_MIN -1000
#define P_TIME_MAX 1000



/******************************************/





#include <string>
#include <iostream>
#include <inttypes.h>
        
#include "../ui_logger/Logger.h"
#include "../lib/pugixml.hpp"
#include "ServerException.h"

enum permissions {
    everyone=1, 
    guest=2, 
    user=3,
    admin=4, 
    superuser=5 
};

namespace proto{
    static const char communicationNode[] = "com";
    static const char versionAttr[] = "version";
    static const char errorCodeAttr[] = "errcode";
    static const char stateAttr[] = "state"; 
    static const char sessionIdAttr[] = "sessionid";
    
    static const char adapterNode[] = "gate";
    static const char deviceNode[] = "device";
    static const char moduleNode[] = "part";
    
    static const char headerGatewayIdAttr[] = "gateid";
    static const char gatewayIdAttr[] = "id";
    static const char gatewayNameAttr[] = "name";
    static const char gatewayUsersRole[] = "accessrole";
    static const char gatewayUtcAttr[] = "utc";
    static const char gatewayIpAttr[] = "ip";
    static const char gatewayVersionAttr[] = "version";
    static const char gatewayInfoNusersAttr[] = "users";
    static const char gatewayInfoNdevicesAttr[] = "devices";
    
    static const char deviceIdAttr[] = "id";
    static const char deviceInitAttr[] = "init";
    static const char deviceNameAttr[] = "name";
    static const char deviceLocationIdAttr[] = "lid";
    
    static const char moduleIdAttr[] = "id";
    static const char moduleTypeAttr[] = "type";
    static const char moduleValueIdAttr[] = "value";
    
    static const char googleCloudMessagingIdAttr[] = "gcmid";

    
    static const char roleGuestAttr[] = "guest";
    static const char roleUserAttr[] = "user";
    static const char roleAdminAttr[] = "admin";
    static const char roleSuperuserAttr[] = "superuser";
    
    static const char userNode[] = "user";
    
    static const char userIdAttr[] = "id";
    static const char userRoleAttr[] = "role";
    static const char userNameAttr[] = "name";
    static const char userSurnameAttr[] = "surname";
    static const char userGenderAttr[] = "gender";
    static const char userEmailAttr[] = "email";
    static const char userPictureAttr[] = "imgurl";
    static const char userAccountsAttr[] = "accounts";
    
    static const char logsNode[] = "logs";
    static const char logsFromAttr[] = "from";
    static const char logsToAttr[] = "to";
    static const char logsFunctionAttr[] = "ftype";
    static const char logsIntervalAttr[] = "interval";
    static const char logsDeviceIdAttr[] = "deviceid";
    static const char logsModuleIdAttr[] = "moduleid";
    
    static const char notificationNode[] = "notification";
    static const char notificationIdAttr[] = "id";
    
    static const char locationNode[] = "location";
    static const char locationIdAttr[] = "id";
    static const char locationNameAttr[] = "name";
    static const char locationTypeAttr[] = "type";
    
    
    static const char authProviderNode[] = "provider";
    static const char authProvideNameAttr[] = "name";
    static const char authProvideTokenAttr[] = "token";
    static const char authProviderIdAttr[] = "id";
    static const char phoneNode[] = "phone";
    static const char phoneNameAttr[] = "name";
    
    
//    static const char authServiceNode[] = "srv";
//    static const char authServiceNameAttr[] = "name";
//    static const char authServiceIdAttr[] = "id";
    static const char authServiceGoogleAttr[] = "google";
    static const char authServiceFacebookAttr[] = "facebook";
    static const char authServiceBeeeonAttr[] = "beeeon";
    
    
    static const char replyTrue[] = "true";
    static const char replyFalse[] = "false";
    static const char replyAccounts[] = "accounts";
    static const char replyAllDevices[] = "alldevices";
    static const char replyDevices[] = "devices";
    static const char replyGates[] = "gates";
    static const char replyGateInfo[] = "gateinfo";
    static const char replyLocationCreated[] = "locationid";
    static const char replyLocations[] = "locations";
    static const char replyLogs[] = "logs";
    static const char replySessionId[] = "sessionid";
    static const char replyUserInfo[] = "userinfo";
    static const char replyNotifications[] = "notifications";
}


#define P_DEVICE_REFRESH_DEPRECATED "refresh"
#define P_DEVICE_BATTERY_DEPRECATED "batt"
#define P_DEVICE_QUALITY_DEPRECATED "rssi"

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
        long long _gatewayId;
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

protected:
    t_version parseProtocolVersion(std::string version);
    std::string envelopeResponseSetAttributes(std::string state, std::string response, std::string attributes);
    
    void makeCommunicationHeader(std::string responseState);
};

#endif /* IMSGIN_H */
