#ifndef BEEEON_POCO_REST_REQUEST_HANDLER_H
#define BEEEON_POCO_REST_REQUEST_HANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "rest/MappedRestAction.h"
#include "rest/RestAction.h"
#include "server/Session.h"
#include "util/Loggable.h"

namespace Poco {
namespace Net {

class HTTPServerRequest;
class HTTPServerResponse;

}
}

namespace BeeeOn {

class RestLinker;
class RestRouter;
class SessionVerifier;

class PocoRestRequestHandler : public Poco::Net::HTTPRequestHandler, Loggable {
public:
	PocoRestRequestHandler(
			RestAction::Ptr action,
			const MappedRestAction::Params &params,
			ExpirableSession::Ptr session,
			RestLinker &linker);

	void handleRequest(
			Poco::Net::HTTPServerRequest &req,
			Poco::Net::HTTPServerResponse &res) override;

protected:
	bool expectedContentLength(
			Poco::Net::HTTPServerRequest &req,
			Poco::Net::HTTPServerResponse &res);

	std::string asString(const MappedRestAction::Params &params) const;

	void prepareInternalAction(
			RestAction::Ptr action,
			Poco::Net::HTTPServerRequest &req,
			Poco::Net::HTTPServerResponse &res) const;

	void prepareMappedAction(
			MappedRestAction::Ptr action,
			Poco::Net::HTTPServerRequest &req,
			Poco::Net::HTTPServerResponse &res) const;

private:
	RestAction::Ptr m_action;
	MappedRestAction::Params m_params;
	ExpirableSession::Ptr m_session;
	RestLinker &m_linker;
};

/**
 * Resolve the request via RestRouter.
 */
class PocoRestRequestFactory : public Poco::Net::HTTPRequestHandlerFactory, Loggable {
public:
	PocoRestRequestFactory(RestRouter &router, SessionVerifier &verifier);

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
};


}

#endif
