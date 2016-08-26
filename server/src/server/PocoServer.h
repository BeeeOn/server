#ifndef BEEEON_POCO_SERVER
#define BEEEON_POCO_SERVER

#include <string>
#include <Poco/Logger.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "server/Server.h"
#include "server/RestRequestHandler.h"
#include "Debug.h"

namespace BeeeOn {

template <typename UserData = void *>
class PocoRestRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
	using RestRequestHandler =
		TRestRequestHandler<
			Poco::Net::HTTPServerRequest,
			Poco::Net::HTTPServerResponse,
			UserData>;

	PocoRestRequestHandler(RestRequestHandler *impl):
		m_impl(impl)
	{
	}

	~PocoRestRequestHandler()
	{
		delete m_impl;
	}

	void handleRequest(Poco::Net::HTTPServerRequest &req,
			Poco::Net::HTTPServerResponse &res)
	{
		m_impl->handleRequest(req, res);
	}

private:
	RestRequestHandler *m_impl;
};

template <typename UserData = void *>
class PocoRestRequestHandlerFactory : 
	public Poco::Net::HTTPRequestHandlerFactory {
public:
	using RestRequestHandlerFactory =
		TRestRequestHandlerFactory<
			Poco::Net::HTTPServerRequest,
			Poco::Net::HTTPServerResponse,
			UserData>;
	using Route = TRoute<
			Poco::Net::HTTPServerRequest,
			Poco::Net::HTTPServerResponse,
			UserData>;
	using Handler = typename Route::Handler;
	using SessionVerifier =
		typename RestRequestHandlerFactory::SessionVerifier;

	PocoRestRequestHandlerFactory(UserData &data, const std::string &name):
		m_impl(data, name)
	{
	}

	Poco::Net::HTTPRequestHandler *createRequestHandler(
			const Poco::Net::HTTPServerRequest &req)
	{
		return new PocoRestRequestHandler<UserData>(
				m_impl.createRequestHandler(req));
	}

	void noRoute(Handler h)
	{
		m_impl.noRoute(h);
	}

	void noOperation(Handler h)
	{
		m_impl.noOperation(h);
	}

	void sessionVerifier(SessionVerifier sessionVerifier)
	{
		m_impl.sessionVerifier(sessionVerifier);
	}

	void POST(const char *pattern, Handler handler, bool secure = true)
	{
		m_impl.POST(pattern, handler, secure);
	}

	void PUT(const char *pattern, Handler handler, bool secure = true)
	{
		m_impl.PUT(pattern, handler, secure);
	}

	void GET(const char *pattern, Handler handler, bool secure = true)
	{
		m_impl.GET(pattern, handler, secure);
	}

	void DELETE(const char *pattern, Handler handler, bool secure = true)
	{
		m_impl.DELETE(pattern, handler, secure);
	}

	void OPTIONS(const char *pattern, Handler handler, bool secure = true)
	{
		m_impl.OPTIONS(pattern, handler, secure);
	}

	void reportRoutes() const
	{
		m_impl.reportRoutes();
	}

private:
	RestRequestHandlerFactory m_impl;
};

template <typename UserData = void *>
class TPocoServer : public Server {
public:
	using RequestHandlerFactory = PocoRestRequestHandlerFactory<UserData>;
	TPocoServer(unsigned int port,
			Poco::SharedPtr<RequestHandlerFactory> factory):
		m_server(factory, port),
		m_factory(factory)
	{
	}

	void start()
	{
		LOGGER_CLASS(this).information(
			"starting server on port "
			+ std::to_string(m_server.port()));

		m_factory->reportRoutes();
		m_server.start();
	}

	void stop()
	{
		LOGGER_CLASS(this).information(
			"stopping server on port "
			+ std::to_string(m_server.port()));

		m_server.stop();

		LOGGER_CLASS(this).information(
			"server on port "
			+ std::to_string(m_server.port()) + " stopped");
	}

private:
	Poco::SharedPtr<RequestHandlerFactory> m_factory;
	Poco::Net::HTTPServer m_server;
};

}

#endif
