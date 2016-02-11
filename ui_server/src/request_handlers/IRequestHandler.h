#ifndef IMSGIN_H
#define	IMSGIN_H

#include <string>
#include <iostream>
#include <inttypes.h>
        
#include "../ui_logger/Logger.h"
#include "../lib/pugixml.hpp"
#include "ServerException.h"

enum permissions 
{
    everyone=1, 
    guest=2, 
    user=3,
    admin=4, 
    owner=5 
};

namespace proto
{
    static const char requestNode[] = "request";
    static const char responseNode[] = "response";
    static const char versionAttr[] = "version";
    static const char errorCodeAttr[] = "errcode";
    static const char namespaceAttr[] = "ns"; 
    static const char typeAttr[] = "type"; 
    static const char resultAttr[] = "result"; 
    static const char sessionIdAttr[] = "sessionid";
    
    static const char gatewayNode[] = "gate";
    static const char deviceNode[] = "device";
    static const char moduleNode[] = "part";
    
    static const char headerGatewayIdAttr[] = "gateid";
    static const char gatewayIdAttr[] = "id";
    static const char gatewayNameAttr[] = "name";
    static const char gatewayDeviceIpAttr[] = "deviceip";
    static const char gatewayDeviceEuidpAttr[] = "deviceeuid";

    static const char gatewayUsersRole[] = "permission";
    static const char gatewayTimezoneAttr[] = "timezone";
    static const char gatewayAltitudeAttr[] = "altitude";
    static const char gatewayLongitudeAttr[] = "longitude";
    static const char gatewayLatitudeAttr[] = "latitude";
    static const char gatewayStatusAttr[] = "status";
    
    static const char gatewayIpAttr[] = "ip";
    static const char gatewayVersionAttr[] = "version";
    static const char gatewayInfoNusersAttr[] = "users";
    static const char gatewayInfoNdevicesAttr[] = "devices";
    static const char gatewayInfoOwner[] = "owner";
    
    static const char deviceIdAttr[] = "euid";
    static const char deviceInitAttr[] = "init";
    static const char deviceNameAttr[] = "name";
    static const char deviceLocationIdAttr[] = "locationid";
    static const char deviceGateIdAttr[] = "gateid";
    static const char deviceModuleIdAttr[] = "moduleid";
    static const char deviceParamKeyAttr[] = "parameterkey";
    static const char deviceParamValueAttr[] = "parametervalue";
    
    static const char moduleIdAttr[] = "id";
    static const char moduleTypeAttr[] = "type";
    static const char moduleValueIdAttr[] = "value";
    
    static const char pushNotificationServiceNode[] = "service";
    static const char pushNotificationServiceNameAttr[] = "name";
    static const char pushNotificationServiceIdAttr[] = "id";
    static const char pushNotificationServiceUserIdAttr[] = "userid";

    static const char roleGuestAttr[] = "guest";
    static const char roleUserAttr[] = "user";
    static const char roleAdminAttr[] = "admin";
    static const char roleOwnerAttr[] = "owner";
    
    static const char userNode[] = "user";
    
    static const char userIdAttr[] = "id";
    static const char userRoleAttr[] = "permission";
    static const char userNameAttr[] = "name";
    static const char userSurnameAttr[] = "surname";
    static const char userGenderAttr[] = "gender";
    static const char userEmailAttr[] = "email";
    static const char userPictureAttr[] = "imgurl";
    static const char userAccountsAttr[] = "providers";
    
    static const char logsNode[] = "logs";
    static const char logsFromAttr[] = "from";
    static const char logsToAttr[] = "to";
    static const char logsFunctionAttr[] = "ftype";
    static const char logsIntervalAttr[] = "interval";
    static const char logsDeviceIdAttr[] = "deviceeuid";
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
    static const char authProvideMailAttr[] = "email";
    static const char authProvidePasswordAttr[] = "password";
    static const char authProviderIdAttr[] = "id";
    static const char phoneNode[] = "phone";
    static const char phoneNameAttr[] = "name";
    
    static const char authServiceGoogleAttr[] = "google";
    static const char authServiceFacebookAttr[] = "facebook";
    static const char authServiceBeeeonAttr[] = "beeeon";
    
    static const char replyTrue[] = "ok";
    static const char replySessionId[] = "sessionid";
    static const char replyFalse[] = "error";
    static const char replyData[] = "data";
    static const char replyAccounts[] = "accounts";
    static const char replyAllDevices[] = "alldevices";
    static const char replyDevices[] = "devices";
    static const char replyGates[] = "gates";
    static const char replyGateInfo[] = "gateinfo";
    static const char replyLocationCreated[] = "locationid";
    static const char replyLocations[] = "locations";
    static const char replyLogs[] = "logs";
    static const char replyUserInfo[] = "userinfo";
    static const char replyNotifications[] = "notifications";
}

struct t_version 
{
    int majorVersion;
    int minorVersion;
    int patchVersion;
};

enum enumAccessStatus { GRANTED, FORBIDDEN_NOT_LOGGED, FORBIDDEN_WRONG_RIGHTS };

class IRequestHandler
{
public:
    IRequestHandler(std::shared_ptr<pugi::xml_document> doc);
    virtual ~IRequestHandler(void);
    virtual std::string createResponseMsgOut() = 0;

    virtual enumAccessStatus checkAccess() = 0;
    virtual bool isComIdValid() = 0;
    bool checkProtocolVersion();
public:
    std::string _namespace;
    std::string _state;
    std::string _token;
protected:
    std::shared_ptr<pugi::xml_document> _doc;
    std::string _msgOut;

    pugi::xml_document _outputDoc;
    pugi::xml_node _outputMainNode;
        
public:
    static const int MAJOR_VERSION;
    static const int MINOR_VERSION;
    static const int PATCH_VERSION;
    static const std::string VERSION;
public:
    std::string getNegativeXMLReply(int errorCode);
protected:
    std::string makeXMLattribute(std::string attr,std::string value);
    std::string getXMLreply(std::string responseState);
    std::string getDataXMLreply(std::string responseState, std::string data);
    std::string getPositiveXMLReply();
    std::string getNegativeXMLReply(int errorCode, const char* errorInfo);
    
    bool isRoleValid(std::string role);

protected:
    t_version parseProtocolVersion(std::string version);
    std::string envelopeResponseSetAttributes(std::string state, std::string response, std::string attributes);
    
    void makeCommunicationHeader(std::string responseState);
};

#endif /* IMSGIN_H */
