#ifndef BEEEON_UI_SERVER_MODULE_H
#define BEEEON_UI_SERVER_MODULE_H

#include <iostream>
#include <Poco/SharedPtr.h>

#include "loop/StoppableLoop.h"
#include "server/RestRequestHandler.h"
#include "server/SessionVerifier.h"
#include "server/RestAuthHandler.h"
#include "di/InjectorTarget.h"
#include "service/UserService.h"
#include "service/DeviceService.h"
#include "util/Loggable.h"

#if defined(BEEEON_SELECT_POCO)

/**
 * Use Poco HTTPServer.
 */
#include "server/PocoServer.h"
#define config_module \
	typedef Poco::Net::HTTPServerRequest  UIRequest; \
	typedef Poco::Net::HTTPServerResponse UIResponse; \
	typedef PocoRestRequestHandlerFactory \
		UIServerRequestHandlerFactory; \
	typedef PocoServer UIRestServer

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
typedef TRoute<UIRequest, UIResponse, ExpirableSession::Ptr> UIRoute;
/**
 * UIRouteContext class built from TRouteContext template.
 */
typedef UIRoute::Context UIRouteContext;

class UIServerModule : public AbstractInjectorTarget,
		public Loggable,
		public StoppableLoop {
public:
	UIServerModule(void):
		m_port(0),
		m_factory(new UIServerRequestHandlerFactory("ui-server")),
		m_server(NULL)
	{
		injector("sessionVerifier", &UIServerModule::setSessionVerifier);
		injector("userService", &UIServerModule::setUserService);
		injector("authHandler", &UIServerModule::setAuthHandler);
		injector("deviceService", &UIServerModule::setDeviceService);
		numberInjector("port", (NumberSetter) &UIServerModule::setPort);
	}

	/**
	 * Called when dependency injection sets all entries.
	 */
	void injectionDone() override;

	~UIServerModule() {
		if (m_server)
			delete m_server;
	}

	void setPort(int port)
	{
		m_port = port;
	}

	void createServer()
	{
		m_server = new UIRestServer(m_port, m_factory);
	}

	UIServerRequestHandlerFactory &factory()
	{
		return *m_factory;
	}

	void start()
	{
		createServer();
		m_server->start();
	}

	void stop()
	{
		m_server->stop();
	}

	void setSessionVerifier(SessionVerifier *verifier)
	{
		m_factory->sessionVerifier(verifier);
	}

	void setUserService(UserService *service)
	{
		m_userService = service;
	}

	UserService &userService()
	{
		return *m_userService;
	}

	void setAuthHandler(RestAuthHandler *handler)
	{
		m_authHandler = handler;
	}

	void setDeviceService(DeviceService *service)
	{
		m_deviceService = service;
	}

	DeviceService &deviceService()
	{
		return *m_deviceService;
	}

private:
	int m_port;
	Poco::SharedPtr<UIServerRequestHandlerFactory> m_factory;
	UIRestServer *m_server;
	UserService *m_userService;
	RestAuthHandler *m_authHandler;
	DeviceService *m_deviceService;
};

}

#endif
