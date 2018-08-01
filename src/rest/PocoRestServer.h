#ifndef BEEEON_POCO_REST_SERVER_H
#define BEEEON_POCO_REST_SERVER_H

#include <Poco/SharedPtr.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>

#include "l10n/Translator.h"
#include "l10n/HTTPLocaleExtractor.h"
#include "server/AbstractServer.h"
#include "server/HTTPFilterChain.h"
#include "ssl/SSLServer.h"
#include "util/HavingThreadPool.h"

namespace BeeeOn {

class RestRouter;
class SessionVerifier;

class PocoRestServer :
		public AbstractServer,
		public HavingThreadPool {
public:
	PocoRestServer();

	/**
	 * Set Rest router for this server.
	 */
	void setRouter(RestRouter *router);

	/**
	 * Set global filter chain applied to each request.
	 */
	void setFilterChain(HTTPFilterChain::Ptr filterChain);

	/**
	 * Set session verifier.
	 */
	void setSessionVerifier(SessionVerifier *verifier);

	/**
	 * Factory to create l10n translators.
	 */
	void setTranslatorFactory(TranslatorFactory::Ptr factory);

	/**
	 * Set locale manager for HTTPLocaleExtractor.
	 */
	void setLocaleManager(Poco::SharedPtr<LocaleManager> manager);

	/**
	 * Set the SSL configuration.
	 */
	void setSSLConfig(SSLServer::Ptr config);

protected:
	void initServerSocket();
	void initFactory();
	void initHttpServer();

	/**
	 * Start the Rest HTTP server.
	 */
	void doStart() override;

	/**
	 * Stop the Rest HTTP server.
	 */
	void doStop() override;

private:
	RestRouter *m_router;
	SessionVerifier *m_sessionVerifier;
	SSLServer::Ptr m_sslConfig;
	Poco::SharedPtr<Poco::Net::ServerSocket> m_socket;
	Poco::Net::HTTPRequestHandlerFactory::Ptr m_factory;
	Poco::SharedPtr<Poco::Net::HTTPServer> m_server;
	TranslatorFactory::Ptr m_translatorFactory;
	HTTPLocaleExtractor m_localeExtractor;
	HTTPFilterChain::Ptr m_filterChain;
};

}

#endif
