#ifndef BEEEON_POCO_REST_REQUEST_HANDLER_H
#define BEEEON_POCO_REST_REQUEST_HANDLER_H

#include <Poco/Clock.h>
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
			RestLinker &linker,
			TranslatorFactory &factory,
			HTTPLocaleExtractor &localeExtractor,
			HTTPFilterChain &filterChain);

	void setStarted(const Poco::Clock &started);
	const Poco::Clock &started() const;

	void setAction(RestAction::Ptr action);
	void setActionParams(const MappedRestAction::Params &params);
	void setSession(Session::Ptr session);

protected:
	void run() override;
	void setupLanguage(RestFlow &flow);
	void doHandleRequest();

	/**
	 * Test whether the request contains Content-Length if required.
	 * When missing it, such request would fail. If the Content-Length
	 * is present and required, check that the value does not exceed
	 * the limit for the resolved action.
	 */
	bool expectedContentLength();

	/**
	 * @return string representation of the given action parameters for
	 * debugging purposes.
	 */
	std::string asString(const MappedRestAction::Params &params) const;

	/**
	 * Prepare the HTTP response for handling the given internal action.
	 * The internal actions are non-cachable.
	 */
	void prepareInternalAction(const RestAction::Ptr action);

	/**
	 * Prepare the HTTP response for handling the given user action.
	 * This usually contains applying of cache headers from the action
	 * settings.
	 */
	void prepareMappedAction(const MappedRestAction::Ptr action);

private:
	RestAction::Ptr m_action;
	MappedRestAction::Params m_params;
	Session::Ptr m_session;
	RestLinker &m_linker;
	TranslatorFactory &m_translatorFactory;
	HTTPLocaleExtractor &m_localeExtractor;
	HTTPFilterChain &m_filterChain;
	Poco::Clock m_started;
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
	PocoRestRequestHandler *createHandler(RestAction::Ptr action);
	PocoRestRequestHandler *routeBuiltin(const std::string &name);
	PocoRestRequestHandler *handleNoRoute(
			const Poco::Net::HTTPServerRequest &request);
	PocoRestRequestHandler *handleNoSession();
	PocoRestRequestHandler *createWithSession(
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
