#include "RequestHandlerFactory.h"

#include "ServerException.h"

#include <algorithm> 

#include "request_handlers/IRequestHandler.h"

#include "request_handlers/AccountsLoginRequestHandler.h"
#include "request_handlers/DevicesGetRequestHandler.h"
#include "request_handlers/UnknownRequestHandler.h"
#include "request_handlers/GatesGetAllRequestHandler.h"
#include "request_handlers/LocationsGetAllRequestHandler.h"
#include "request_handlers/DevicesGetLogRequestHandler.h"
#include "request_handlers/LocationsAddRequestHandler.h"
#include "request_handlers/LocationsDeleteRequestHandler.h"
#include "request_handlers/LocationsUpdateRequestHandler.h"
#include "request_handlers/GateusersRemoveRequestHandler.h"
#include "request_handlers/GateusersUpdateRequestHandler.h"
#include "request_handlers/GateusersInviteRequestHandler.h"
#include "request_handlers/DevicesGetAllRequestHandler.h"
#include "request_handlers/DevicesUnregisterRequestHandler.h"
#include "request_handlers/GatesStartListenRequestHandler.h"
#include "request_handlers/DevicesSetStateRequestHandler.h"
#include "request_handlers/GatesRegisterRequestHandler.h"
#include "request_handlers/DevicesGetNewRequestHandler.h"
#include "request_handlers/NotificationRegisterServiceRequestHandler.h"
#include "request_handlers/NotificationsUnregisterServiceRequestHandler.h"
#include "request_handlers/NotificationsGetRequestHandler.h"
#include "request_handlers/NotificationsSetReadRequestHandler.h"
#include "request_handlers/AccountGetProfileRequestHandler.h"
#include "request_handlers/AccountsRegisterRequestHandler.h"
#include "request_handlers/GatesUnregisterRequestHandler.h"
#include "request_handlers/AccountsLogoutRequestHandler.h"
#include "request_handlers/GatesGetRequestHandler.h"
#include "request_handlers/DevicesUpdateRequestHandler.h"
#include "request_handlers/GatesUpdateRequestHandler.h"
#include "request_handlers/GateusersGetAllRequestHandler.h"
#include "DAO/DAODevices.h"
#include "DAO/DAOlogs.h"
#include "DAO/DAOLocations.h"
#include "DAO/DAOPushNotificationService.h"
#include "DAO/DAONotification.h"
#include "Config.h"
#include "DAO/DAOContainer.h"
#include "request_handlers/DevicesGetParameterRequestHandler.h"
#include "request_handlers/DevicesDeleteParameterRequestHandler.h"
#include "request_handlers/DevicesCreateParameterRequestHandler.h"
#include "request_handlers/DevicesUpdateParameterRequestHandler.h"
#include "request_handlers/GatesSearchRequestHandler.h"

using namespace std;

RequestHandlerFactory::RequestHandlerFactory(DAOContainer & daoContainer, AdaServerCommunicator & adaServerCom, SessionsTable & sessionTable)
    :   _daoUsers(daoContainer.getDaoUsers()),
        _daoGateways(daoContainer.getDaoGateway()),
        _daoDevices(daoContainer.getDaoDevices()),
        _daoLocations(daoContainer.getDaoLocations()),
        _daoMobileDevices(daoContainer.getDaoMobileDevices()),
        _daoUsersGateways(daoContainer.getDaoUsersGateway()),
        _daoLogs(daoContainer.getDaoLogs()),
        _daoPushNofitication(daoContainer.getDaoPushNotification()),
        _daoNotification(daoContainer.getDaoNotification()),
        _daoDevPar(daoContainer.getDaoDevicesParameters()),
        _adaServerCom(adaServerCom),
        _sessionTable(sessionTable)
{
}

RequestHandlerFactory::~RequestHandlerFactory(void)
{
}


shared_ptr<IRequestHandler> RequestHandlerFactory::createMsg(const char* msg)
{      
            
    shared_ptr<pugi::xml_document> doc(new pugi::xml_document());
    pugi::xml_parse_result result = doc.get()->load(msg);
            
        
    if (!result)
    {
        Logger::error()<< "XML [" << msg << "] parsed with errors" << endl;
        Logger::error()<< "Error description: " << result.description() << endl;
        Logger::error() << "Error offset: " << result.offset << " (error at [..." << (msg + result.offset) << "]"<<endl;
        return shared_ptr<IRequestHandler>(new UnknownRequestHandler(doc));
    }

    string type = doc->child(proto::requestNode).attribute(proto::typeAttr).value();
    string ns = doc->child(proto::requestNode).attribute(proto::namespaceAttr).value();
    
    Logger::getInstance(Logger::DEBUG3) << "factory: creating msg "<< type<< endl;

    if(ns == "accounts")
    {
        if(type == AccountsRegisterRequestHandler::state)
            return shared_ptr<IRequestHandler>(new AccountsRegisterRequestHandler(doc, _daoUsers));
        if(type == AccountsLoginRequestHandler::state)
            return shared_ptr<IRequestHandler>(new AccountsLoginRequestHandler(doc, _daoUsers, _sessionTable));
        if(type == AccountsLogoutRequestHandler::state)
            return shared_ptr<IRequestHandler>(new AccountsLogoutRequestHandler(doc, _daoMobileDevices, _sessionTable));
        if(type == AccountGetProfileRequestHandler::state)
            return shared_ptr<IRequestHandler>(new AccountGetProfileRequestHandler(doc, _daoUsers, _sessionTable));  
    }
    else if(ns == "gateusers")
    {
        if(type == GateusersGetAllRequestHandler::state)        
            return shared_ptr<IRequestHandler>(new GateusersGetAllRequestHandler(doc, _daoUsers, _daoUsersGateways, _sessionTable));
        if(type == GateusersRemoveRequestHandler::state)
            return shared_ptr<IRequestHandler>(new GateusersRemoveRequestHandler(doc, _daoUsers, _daoUsersGateways, _sessionTable));
        if(type == GateusersUpdateRequestHandler::state)
            return shared_ptr<IRequestHandler>(new GateusersUpdateRequestHandler(doc, _daoUsers, _daoUsersGateways, _sessionTable));
        if(type == GateusersInviteRequestHandler::state)
            return shared_ptr<IRequestHandler>(new GateusersInviteRequestHandler(doc, _daoUsers, _daoUsersGateways, _sessionTable));
    }
    else if(ns == "gates")
    {
        if(type == GatesStartListenRequestHandler::state)
            return shared_ptr<IRequestHandler>(new GatesStartListenRequestHandler(doc, _daoUsers, _adaServerCom, _sessionTable));
        if(type == GatesRegisterRequestHandler::state)
            return shared_ptr<IRequestHandler>(new GatesRegisterRequestHandler(doc, _daoUsers, _daoGateways, _daoUsersGateways, _sessionTable));
        if(type == GatesGetRequestHandler::state)
            return shared_ptr<IRequestHandler>(new GatesGetRequestHandler(doc, _daoUsers, _daoGateways, _sessionTable));
        if(type == GatesUpdateRequestHandler::state)
            return shared_ptr<IRequestHandler>(new GatesUpdateRequestHandler(doc, _daoUsers, _daoGateways, _sessionTable)); 
        if(type == GatesUnregisterRequestHandler::state)
            return shared_ptr<IRequestHandler>(new GatesUnregisterRequestHandler(doc, _daoUsers, _daoGateways, _daoUsersGateways, _sessionTable));
        if(type == GatesGetAllRequestHandler::state)
            return shared_ptr<IRequestHandler>(new GatesGetAllRequestHandler(doc, _daoUsers, _sessionTable));
        if(type == GatesSearchRequestHandler::state)
            return shared_ptr<IRequestHandler>(new GatesSearchRequestHandler(doc, _daoUsers, _adaServerCom, _sessionTable));
    }
    else if(ns == "locations")
    {
        if(type == LocationsGetAllRequestHandler::state)
            return shared_ptr<IRequestHandler>(new LocationsGetAllRequestHandler(doc, _daoUsers, _daoLocations, _sessionTable));
        if(type == LocationsAddRequestHandler::state)
            return shared_ptr<IRequestHandler>(new LocationsAddRequestHandler(doc, _daoUsers, _daoLocations, _sessionTable));
        if(type == LocationsDeleteRequestHandler::state)
            return shared_ptr<IRequestHandler>(new LocationsDeleteRequestHandler(doc, _daoUsers, _daoLocations, _sessionTable));
        if(type == LocationsUpdateRequestHandler::state)
            return shared_ptr<IRequestHandler>(new LocationsUpdateRequestHandler(doc, _daoUsers, _daoLocations, _sessionTable));
    }
    else if(ns == "devices")
    {
        if(type == DevicesGetNewRequestHandler::state)
            return shared_ptr<IRequestHandler>(new DevicesGetNewRequestHandler(doc, _daoUsers, _daoDevices, _sessionTable));
        if(type == DevicesGetAllRequestHandler::state)
            return shared_ptr<IRequestHandler>(new DevicesGetAllRequestHandler(doc, _daoUsers, _daoDevices, _sessionTable));
        if(type == DevicesGetRequestHandler::state)
            return shared_ptr<IRequestHandler>(new DevicesGetRequestHandler(doc, _daoDevices, _sessionTable));
        if(type == DevicesGetLogRequestHandler::state)
            return shared_ptr<IRequestHandler>(new DevicesGetLogRequestHandler(doc, _daoUsers, _daoLogs, _sessionTable));
        if(type == DevicesUnregitersRequestHandler::state)
            return shared_ptr<IRequestHandler>(new DevicesUnregitersRequestHandler(doc, _daoUsers, _adaServerCom, _sessionTable));
        if(type == DevicesUpdateRequestHandler::state)
            return shared_ptr<IRequestHandler>(new DevicesUpdateRequestHandler(doc, _daoUsers, _daoDevices, _sessionTable));
        if(type == DevicesSetStateRequestHandler::state)
            return shared_ptr<IRequestHandler>(new DevicesSetStateRequestHandler(doc, _daoUsers, _adaServerCom, _sessionTable));
        if(type == DevicesGetParameterRequestHandler::state)
            return shared_ptr<IRequestHandler>(new DevicesGetParameterRequestHandler(doc, _daoUsers, _daoDevPar, _sessionTable));
        if(type == DevicesDeleteParameterRequestHandler::state)
            return shared_ptr<IRequestHandler>(new DevicesDeleteParameterRequestHandler(doc, _daoUsers, _daoDevPar, _sessionTable));
        if(type == DevicesCreateParameterRequestHandler::state)
            return shared_ptr<IRequestHandler>(new DevicesCreateParameterRequestHandler(doc, _daoUsers, _daoDevPar, _sessionTable));
        if(type == DevicesUpdateParameterRequestHandler::state)
            return shared_ptr<IRequestHandler>(new DevicesUpdateParameterRequestHandler(doc, _daoUsers, _daoDevPar, _sessionTable));
    }
    else if(ns == "notifications")
    {
        if(type == NotificationRegisterServiceRequestHandler::state)
            return shared_ptr<IRequestHandler>(new NotificationRegisterServiceRequestHandler(doc, _daoPushNofitication, _sessionTable));
        if(type == NotificationsUnregisterServiceRequestHandler::state)
            return shared_ptr<IRequestHandler>(new NotificationsUnregisterServiceRequestHandler(doc, _daoPushNofitication));
        if(type == NotificationsGetRequestHandler::state)
            return shared_ptr<IRequestHandler>(new NotificationsGetRequestHandler(doc, _daoNotification, _sessionTable));
        if(type == NotificationsSetReadRequestHandler::state)
            return shared_ptr<IRequestHandler>(new NotificationsSetReadRequestHandler(doc, _daoNotification, _sessionTable));  
    }

    Logger::getInstance(Logger::ERROR)<<"UNKNOWN MSG (ns: " << ns << " type: " << type <<  ")" <<endl;
    return shared_ptr<IRequestHandler>(new UnknownRequestHandler(doc));
}
