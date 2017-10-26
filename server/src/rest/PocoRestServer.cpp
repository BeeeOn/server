#include <Poco/Exception.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/SecureServerSocket.h>

#include "di/Injectable.h"
#include "l10n/Translator.h"
#include "l10n/NoTranslator.h"
#include "server/SessionVerifier.h"
#include "ssl/SSLServer.h"
#include "rest/PocoRestRequestHandler.h"
#include "rest/PocoRestServer.h"
#include "rest/RestRouter.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PocoRestServer)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_REF("router", &PocoRestServer::setRouter)
BEEEON_OBJECT_REF("filterChain", &PocoRestServer::setFilterChain)
BEEEON_OBJECT_REF("sessionVerifier", &PocoRestServer::setSessionVerifier)
BEEEON_OBJECT_REF("translatorFactory", &PocoRestServer::setTranslatorFactory)
BEEEON_OBJECT_REF("localeManager", &PocoRestServer::setLocaleManager)
BEEEON_OBJECT_REF("sslConfig", &PocoRestServer::setSSLConfig)
BEEEON_OBJECT_NUMBER("port", &PocoRestServer::setPort)
BEEEON_OBJECT_NUMBER("backlog", &PocoRestServer::setBacklog)
BEEEON_OBJECT_NUMBER("minThreads", &PocoRestServer::setMinThreads)
BEEEON_OBJECT_NUMBER("maxThreads", &PocoRestServer::setMaxThreads)
BEEEON_OBJECT_TIME("threadIdleTime", &PocoRestServer::setThreadIdleTime)
BEEEON_OBJECT_END(BeeeOn, PocoRestServer)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

PocoRestServer::PocoRestServer():
	m_port(80),
	m_backlog(64),
	m_router(NULL),
	m_sessionVerifier(NULL),
	m_filterChain(new HTTPFilterChain)
{
}

void PocoRestServer::setSSLConfig(SharedPtr<SSLServer> config)
{
	m_sslConfig = config;
}

void PocoRestServer::initServerSocket()
{
	if (!m_socket.isNull())
		return;

	if (m_sslConfig.isNull()) {
		m_socket = new ServerSocket(m_port, m_backlog);
	}
	else {
		Context::Ptr context = m_sslConfig->context();
		m_socket = new SecureServerSocket(m_port, m_backlog, context);
	}
}

void PocoRestServer::initFactory()
{
	if (!m_factory.isNull())
		return;

	if (m_translatorFactory.isNull())
		m_translatorFactory = new NoTranslatorFactory;

	m_factory = new PocoRestRequestFactory(*m_router, *m_sessionVerifier, *m_translatorFactory, m_localeExtractor, m_filterChain);
}

void PocoRestServer::initHttpServer()
{
	if (!m_server.isNull())
		return;

	initServerSocket();
	initFactory();

	HTTPServerParams::Ptr params(new HTTPServerParams);

	m_server = new HTTPServer(m_factory, pool(), *m_socket, params);
}

void PocoRestServer::start()
{
	initHttpServer();
	m_server->start();
}

void PocoRestServer::stop()
{
	if (m_server.isNull())
		return;

	m_server->stop();

	m_server = NULL;
	m_factory = NULL;
	m_socket = NULL;
}

void PocoRestServer::setRouter(RestRouter *router)
{
	m_router = router;
}

void PocoRestServer::setFilterChain(HTTPFilterChain::Ptr filterChain)
{
	m_filterChain = filterChain;
}

void PocoRestServer::setSessionVerifier(SessionVerifier *verifier)
{
	m_sessionVerifier = verifier;
}

void PocoRestServer::setTranslatorFactory(TranslatorFactory::Ptr factory)
{
	m_translatorFactory = factory;
}

void PocoRestServer::setLocaleManager(SharedPtr<LocaleManager> manager)
{
	m_localeExtractor.setLocaleManager(manager);
}

void PocoRestServer::setPort(int port)
{
	if (port < 0)
		throw InvalidArgumentException("port must be non-negative");

	m_port = port;
}

void PocoRestServer::setBacklog(int backlog)
{
	if (backlog < 1)
		throw InvalidArgumentException("backlog must be positive");

	m_backlog = backlog;
}
