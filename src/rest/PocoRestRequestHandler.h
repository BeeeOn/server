#ifndef BEEEON_POCO_REST_REQUEST_HANDLER_H
#define BEEEON_POCO_REST_REQUEST_HANDLER_H

#include <Poco/Net/AbstractHTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "rest/MappedRestAction.h"
#include "rest/RestAction.h"
#include "server/HTTPFilterChain.h"
#include "server/Session.h"
#include "util/Loggable.h"

namespace Poco {
namespace Net {

class HTTPServerRequest;
class HTTPServerResponse;

}
}

namespace BeeeOn {

class HTTPLocaleExtractor;
class RestLinker;
class RestRouter;
class SessionVerifier;
class TranslatorFactory;

class PocoRestRequestHandler : public Poco::Net::AbstractHTTPRequestHandler, Loggable {
public:
	PocoRestRequestHandler(
			const MappedRestAction::Params &params,
			ExpirableSession::Ptr session,
			RestLinker &linker,
			TranslatorFactory &factory,
			HTTPLocaleExtractor &localeExtractor,
			HTTPFilterChain &filterChain);

	void setAction(RestAction::Ptr action);

protected:
	void run() override;
	void doHandleRequest();

	bool expectedContentLength();

	std::string asString(const MappedRestAction::Params &params) const;

	void prepareInternalAction(const RestAction::Ptr action);

	void prepareMappedAction(const MappedRestAction::Ptr action);

private:
	RestAction::Ptr m_action;
	MappedRestAction::Params m_params;
	ExpirableSession::Ptr m_session;
	RestLinker &m_linker;
	TranslatorFactory &m_translatorFactory;
	HTTPLocaleExtractor &m_localeExtractor;
	HTTPFilterChain &m_filterChain;
};

/**
 * Resolve the request via RestRouter.
 */
class PocoRestRequestFactory : public Poco::Net::HTTPRequestHandlerFactory, Loggable {
public:
	PocoRestRequestFactory(RestRouter &router, SessionVerifier &verifier,
			TranslatorFactory &factory, HTTPLocaleExtractor &localeExtractor,
			HTTPFilterChain::Ptr filterChain);

	Poco::Net::HTTPRequestHandler *createRequestHandler(
			const Poco::Net::HTTPServerRequest &request) override;

protected:
	Poco::Net::HTTPRequestHandler *handleNoRoute(
			const Poco::Net::HTTPServerRequest &request);
	Poco::Net::HTTPRequestHandler *handleNoSession();
	Poco::Net::HTTPRequestHandler *createWithSession(
			RestAction::Ptr action,
			const MappedRestAction::Params &params,
			const Poco::Net::HTTPServerRequest &request);

private:
	RestRouter &m_router;
	SessionVerifier &m_sessionVerifier;
	TranslatorFactory &m_translatorFactory;
	HTTPLocaleExtractor &m_localeExtractor;
	HTTPFilterChain::Ptr m_filterChain;
};


}

#endif
