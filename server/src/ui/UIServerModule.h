#ifndef BEEEON_UI_SERVER_MODULE_H
#define BEEEON_UI_SERVER_MODULE_H

#include <iostream>
#include <Poco/SharedPtr.h>

#include "server/RestRequestHandler.h"

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

class UIServerModule {
public:
	UIServerModule(unsigned int port):
		m_factory(new UIServerRequestHandlerFactory(
				*this, "ui-server")),
		m_server(port, m_factory)
	{
		factorySetup(*m_factory);
	}

	~UIServerModule() {}

	UIServerRequestHandlerFactory &factory()
	{
		return *m_factory;
	}

	Server &server()
	{
		return m_server;
	}

private:
	Poco::SharedPtr<UIServerRequestHandlerFactory> m_factory;
	UIRestServer m_server;
};

}

#endif
