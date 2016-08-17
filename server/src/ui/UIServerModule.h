#ifndef BEEEON_UI_SERVER_MODULE_H
#define BEEEON_UI_SERVER_MODULE_H

#include <iostream>
#include <Poco/SharedPtr.h>

#include "server/RestRequestHandler.h"
#include "server/SessionManager.h"
#include "server/SessionManager.h"
#include "di/InjectorTarget.h"
#include "service/UserService.h"
#include "service/AuthService.h"
#include "service/DeviceService.h"
#include "service/GatewayService.h"
#include "service/LocationService.h"

/**
 * Compilation-time configuration of the target underlying
 * HTTP server implementation.
 */
#if defined(BEEEON_SELECT_MONGOOSE)

/**
 * Use Mongoose server.
 */
#include "mongoose/MongooseRequest.h"
#include "mongoose/MongooseResponse.h"
#include "server/MongooseServer.h"
#define config_module \
typedef MongooseRequest UIRequest; \
typedef MongooseResponse UIResponse; \
typedef TRestRequestHandlerFactory \
	<UIRequest, UIResponse, UIServerModule> \
	UIServerRequestHandlerFactory; \
typedef TMongooseServer<UIServerModule> UIRestServer

#elif defined(BEEEON_SELECT_POCO)

/**
 * Use Poco HTTPServer.
 */
#include "server/PocoServer.h"
#define config_module \
	typedef Poco::Net::HTTPServerRequest  UIRequest; \
	typedef Poco::Net::HTTPServerResponse UIResponse; \
	typedef PocoRestRequestHandlerFactory<UIServerModule> \
		UIServerRequestHandlerFactory; \
	typedef TPocoServer<UIServerModule> UIRestServer

#else

#error No server implementation selected

#endif

namespace BeeeOn {

class UIServerModule;

/**
 * Import module configuration, e.g. server settings.
 * This can change the HTTP server backend without
 * affecting any other code.
 */
config_module;

/**
 * UIRoute class built from TRoute template.
 */
typedef TRoute<UIRequest, UIResponse, UIServerModule> UIRoute;
/**
 * UIRouteContext class built from TRouteContext template.
 */
typedef UIRoute::Context UIRouteContext;

void factorySetup(UIServerRequestHandlerFactory &factory);

class UIServerModule : public AbstractInjectorTarget {
public:
	UIServerModule(void):
		m_factory(new UIServerRequestHandlerFactory(*this, "ui-server")),
		m_logger(LOGGER_CLASS(this))
	{
		factorySetup(*m_factory);
		injector<UIServerModule, SessionManager>("sessionManager",
				&UIServerModule::setSessionManager);
		injector<UIServerModule, UserService>("userService",
				&UIServerModule::setUserService);
		injector<UIServerModule, AuthService>("authService",
				&UIServerModule::setAuthService);
		injector<UIServerModule, DeviceService>("deviceService",
				&UIServerModule::setDeviceService);
		injector<UIServerModule, GatewayService>("gatewayService",
				&UIServerModule::setGatewayService);
		injector<UIServerModule, LocationService>("locationService",
				&UIServerModule::setLocationService);
	}

	~UIServerModule() {
		if (m_server)
			delete m_server;
	}

	void createServer(unsigned int port)
	{
		m_server = new UIRestServer(port, m_factory);
	}

	UIServerRequestHandlerFactory &factory()
	{
		return *m_factory;
	}

	Server &server()
	{
		return *m_server;
	}

	void setSessionManager(SessionManager *manager)
	{
		m_sessionManager = manager;
	}

	SessionManager &sessionManager()
	{
		return *m_sessionManager;
	}

	void setUserService(UserService *service)
	{
		m_userService = service;
	}

	UserService &userService()
	{
		return *m_userService;
	}

	void setAuthService(AuthService *service)
	{
		m_authService = service;
	}

	AuthService &authService()
	{
		return *m_authService;
	}

	void setDeviceService(DeviceService *service)
	{
		m_deviceService = service;
	}

	DeviceService &deviceService()
	{
		return *m_deviceService;
	}

	void setGatewayService(GatewayService *service)
	{
		m_gatewayService = service;
	}

	GatewayService &gatewayService()
	{
		return *m_gatewayService;
	}

	void setLocationService(LocationService *service)
	{
		m_locationService = service;
	}

	LocationService &locationService()
	{
		return *m_locationService;
	}

	Poco::Logger &logger()
	{
		return m_logger;
	}

private:
	Poco::SharedPtr<UIServerRequestHandlerFactory> m_factory;
	UIRestServer *m_server;
	SessionManager *m_sessionManager;
	UserService *m_userService;
	AuthService *m_authService;
	DeviceService *m_deviceService;
	GatewayService *m_gatewayService;
	LocationService *m_locationService;
	Poco::Logger &m_logger;
};

}

#endif
