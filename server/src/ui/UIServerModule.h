#ifndef BEEEON_UI_SERVER_MODULE_H
#define BEEEON_UI_SERVER_MODULE_H

#include <iostream>
#include <Poco/SharedPtr.h>

#include "server/RestRequestHandler.h"
#include "server/MongooseServer.h"
#include "mongoose/MongooseRequest.h"
#include "mongoose/MongooseResponse.h"
#include "mongoose/Mongoose.h"

namespace BeeeOn {

class UIServerModule;

typedef MongooseRequest  UIRequest;
typedef MongooseResponse UIResponse;

/**
 * UIRoute class built from TRoute template.
 */
typedef TRoute<UIRequest, UIResponse, UIServerModule> UIRoute;
/**
 * UIRouteContext class built from TRouteContext template.
 */
typedef UIRoute::Context UIRouteContext;

typedef TRestRequestHandlerFactory
	<UIRequest, UIResponse, UIServerModule>
	UIServerRequestHandlerFactory;

typedef TMongooseServer<UIServerModule> UIRestServer;

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
