#ifndef BEEEON_ROUTED_REQUEST_HANDLER_H
#define BEEEON_ROUTED_REQUEST_HANDLER_H

#include <Poco/Exception.h>
#include <Poco/Net/NetException.h>
#include <Poco/Logger.h>
#include <vector>
#include "server/Session.h"
#include "server/SessionVerifier.h"
#include "server/Route.h"
#include "util/Loggable.h"
#include "Debug.h"

namespace BeeeOn {

template <typename Request, typename Response>
class TRestRequestHandlerFactory;

/**
 * Template class RequestHandler. It handles requests from clients
 * and generates appropriate responses. The TRestRequestHandler
 * uses Route class to define actions for URI patterns.
 */
template <typename Request, typename Response>
class TRestRequestHandler : public Loggable {
	friend TRestRequestHandlerFactory<Request, Response>;
public:
	typedef TRestRequestHandlerFactory<Request, Response> Factory;

	using Route = TRoute<Request, Response, ExpirableSession::Ptr>;
	using Params = typename Route::Params;
	using RouteContext = TRouteContext<Request, Response, ExpirableSession::Ptr>;

	TRestRequestHandler(const Route &route,
			const Params &params,
			const std::string &name,
			SessionVerifier *sessionVerifier = NULL):
		m_route(route),
		m_params(params),
		m_name(name),
		m_sessionVerifier(sessionVerifier)
	{
	}

	~TRestRequestHandler()
	{
	}

	void handleNoContentLength(Request &req)
	{
		const std::string &method = req.getMethod();

		if (!method.compare("POST") || !method.compare("PUT")) {
			if (!req.hasContentLength())
				throw Poco::InvalidArgumentException(
						"no Content-Length header");
		}
	}

	void handleRequest(Request &req, Response &res)
	{
		_TRACE_METHOD(logger());

		try {
			res.setVersion(Response::HTTP_1_1);
			res.setStatusAndReason(Response::HTTP_OK);
			m_route.debug(logger(), m_params, __FILE__, __LINE__);

			handleNoContentLength(req);

			ExpirableSession::Ptr session =
				sessionVerify(req, m_route);

			m_route.execute(req, res, m_params, session);
		}
		catch (Poco::NotFoundException &e) {
			logger().log(e, __FILE__, __LINE__);
			res.setStatusAndReason(Response::HTTP_NOT_FOUND);
		}
		catch (Poco::InvalidAccessException &e) {
			logger().log(e, __FILE__, __LINE__);
			res.setStatusAndReason(Response::HTTP_FORBIDDEN);
		}
		catch (Poco::Net::NotAuthenticatedException &e) {
			logger().log(e, __FILE__, __LINE__);
			res.requireAuthentication(m_name);
		}
		catch (Poco::SyntaxException &e) {
			logger().log(e, __FILE__, __LINE__);
			res.setStatusAndReason(
				Response::HTTP_BAD_REQUEST);
		}
		catch (Poco::InvalidArgumentException &e) {
			logger().log(e, __FILE__, __LINE__);
			res.setStatusAndReason(
				Response::HTTP_BAD_REQUEST);
		}
		catch (Poco::Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			res.setStatusAndReason(
				Response::HTTP_INTERNAL_SERVER_ERROR);
		}
		catch (std::exception &e) {
			logger().critical(e.what(), __FILE__, __LINE__);
			res.setStatusAndReason(
				Response::HTTP_INTERNAL_SERVER_ERROR);
			res.setReason("Server internal error");
		}
		catch (const char *s) {
			logger().critical(s, __FILE__, __LINE__);
			res.setStatusAndReason(
				Response::HTTP_INTERNAL_SERVER_ERROR);
			res.setReason("Server internal error");
		}
		catch (...) {
			logger().critical("unknown error, caught '...'",
					__FILE__, __LINE__);
			res.setStatusAndReason(
				Response::HTTP_INTERNAL_SERVER_ERROR);
			res.setReason("Server internal error");
		}

		if (logger().debug()) {
			logger().debug("result: " + req.getURI() + " -> "
				+ std::to_string(res.getStatus()),
				__FILE__, __LINE__);
		}

		if (!res.sent())
			res.send();
	}

protected:
	ExpirableSession::Ptr sessionVerify(const Request &req,
			const Route &route)
	{
		if (!route.isSecure())
			return NULL;

		if (m_sessionVerifier == NULL) {
			logger().warning("no session verifier installed, bypassing");
			return NULL;
		}

		return m_sessionVerifier->verifyAuthorized(req);
	}

protected:
	const Route &m_route;
	const typename Route::Params m_params;
	const std::string &m_name;
	SessionVerifier *m_sessionVerifier;
};

/**
 * Template factory class to create TRestRequestHandlers.
 */
template <typename Request, typename Response>
class TRestRequestHandlerFactory : public Loggable {
public:
	using RequestHandler = TRestRequestHandler<Request, Response>;
	using Route = TRoute<Request, Response, ExpirableSession::Ptr>;
	using RouteVector = typename std::vector<Route>;
	using RouteVectorIterator = typename RouteVector::iterator;
	using Handler = typename Route::Handler;
	using Params = typename Route::Params;

	TRestRequestHandlerFactory(const std::string &name,
			SessionVerifier *sessionVerifier = NULL):
		m_noOperation(new Route("*", NULL, false)),
		m_noRoute(new Route("*", NULL, false)),
		m_name(name),
		m_sessionVerifier(sessionVerifier)
	{
	}

	TRestRequestHandlerFactory(
			Handler noRouteHandler,
			Handler noOperationHandler,
			const std::string &name,
			SessionVerifier *sessionVerifier = NULL):
		m_noOperation(new Route("*", noOperationHandler, false)),
		m_noRoute(new Route("*", noRouteHandler, false)),
		m_name(name),
		m_sessionVerifier(sessionVerifier)
	{
	}

	~TRestRequestHandlerFactory()
	{
		if (m_noOperation != NULL)
			delete m_noOperation;
		if (m_noRoute != NULL)
			delete m_noRoute;
	}

	void noRoute(Handler h)
	{
		delete m_noRoute;
		m_noRoute = new Route("*", h, false);
	}

	void noOperation(Handler h)
	{
		delete m_noOperation;
		m_noOperation = new Route("*", h, false);
	}

	void sessionVerifier(SessionVerifier *sessionVerifier)
	{
		m_sessionVerifier = sessionVerifier;
	}

	void POST(const char *pattern, Handler handler, bool secure = true)
	{
		Route route(pattern, handler, secure);
		logger().information("register POST %s", route.getURI());
		m_POST.push_back(route);
	}

	void PUT(const char *pattern, Handler handler, bool secure = true)
	{
		Route route(pattern, handler, secure);
		logger().information("register PUT %s", route.getURI());
		m_PUT.push_back(route);
	}

	void GET(const char *pattern, Handler handler, bool secure = true)
	{
		Route route(pattern, handler, secure);
		logger().information("register GET %s", route.getURI());
		m_GET.push_back(route);
	}

	void DELETE(const char *pattern, Handler handler, bool secure = true)
	{
		Route route(pattern, handler, secure);
		logger().information("register DELETE %s", route.getURI());
		m_DELETE.push_back(route);
	}

	void OPTIONS(const char *pattern, Handler handler, bool secure = true)
	{
		Route route(pattern, handler, secure);
		logger().information("register OPTIONS %s", route.getURI());
		m_OPTIONS.push_back(route);
	}

	RequestHandler *createRequestHandler(const Request &req)
	{
		TRACE_METHOD();

		if (!req.getMethod().compare("POST"))
			return createRequestHandler(req, m_POST);
		if (!req.getMethod().compare("PUT"))
			return createRequestHandler(req, m_PUT);
		if (!req.getMethod().compare("GET"))
			return createRequestHandler(req, m_GET);
		if (!req.getMethod().compare("DELETE"))
			return createRequestHandler(req, m_DELETE);
		if (!req.getMethod().compare("OPTIONS"))
			return createRequestHandler(req, m_OPTIONS);

		logger().debug("no operation %s %s",
				req.getMethod(),
				req.getURI());
		Params noParams;
		RequestHandler *handler = new RequestHandler(
				*m_noOperation, noParams, m_name);
		return handler;
	}

	void reportRoutes() const
	{
		logger().information("POST routes: %lu", m_POST.size());
		logger().information("PUT routes: %lu", m_PUT.size());
		logger().information("GET routes: %lu", m_GET.size());
		logger().information("DELETE routes: %lu", m_DELETE.size());
		logger().information("OPTIONS routes: %lu", m_OPTIONS.size());
	}

protected:

	RequestHandler *routeMatch(const Request &req, const Route &route)
	{
		Params params;

		if (!route.match(req.getURI(), params))
			return NULL;

		logger().debug("handle %s %s",
				req.getMethod(),
				req.getURI());

		return new RequestHandler(route, params,
				m_name, m_sessionVerifier);
	}

	RequestHandler *createRequestHandler(const Request &req,
			std::vector<Route> &routes)
	{
		RouteVectorIterator it;
		RequestHandler *handler;

		for (it = routes.begin(); it != routes.end(); ++it) {
			handler = routeMatch(req, *it);
			if (handler != NULL)
				return handler;
		}

		logger().debug("no route %s %s",
				req.getMethod(),
				req.getURI());
		Params noParams;
		handler = new RequestHandler(*m_noRoute, noParams, m_name);
		return handler;
	}

private:
	RouteVector m_POST;
	RouteVector m_PUT;
	RouteVector m_GET;
	RouteVector m_DELETE;
	RouteVector m_OPTIONS;
	Route *m_noOperation;
	Route *m_noRoute;
	const std::string m_name;
	SessionVerifier *m_sessionVerifier;
};

}

#endif
