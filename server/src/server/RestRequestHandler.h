#ifndef BEEEON_ROUTED_REQUEST_HANDLER_H
#define BEEEON_ROUTED_REQUEST_HANDLER_H

#include <Poco/Exception.h>
#include <Poco/Net/NetException.h>
#include <Poco/Logger.h>
#include <vector>
#include "server/Route.h"
#include "Debug.h"

namespace BeeeOn {

template <typename Request, typename Response, typename UserData>
class TRestRequestHandlerFactory;

/**
 * Template class RequestHandler. It handles requests from clients
 * and generates appropriate responses. The TRestRequestHandler
 * uses Route class to define actions for URI patterns.
 */
template <typename Request, typename Response, typename UserData = void *>
class TRestRequestHandler {
	friend TRestRequestHandlerFactory<Request, Response, UserData>;
public:
	typedef TRestRequestHandlerFactory
		<Request, Response, UserData> Factory;

	using Route = TRoute<Request, Response, UserData>;
	using Params = typename Route::Params;
	using RouteContext = TRouteContext<Request, Response, UserData>;

	TRestRequestHandler(const Route &route,
			const Params &params,
			UserData &userData,
			const std::string &name):
		m_route(route),
		m_params(params),
		m_userData(userData),
		m_name(name),
		m_logger(LOGGER_CLASS(this))
	{
	}

	~TRestRequestHandler()
	{
	}

	void handleRequest(Request &req, Response &res)
	{
		_TRACE_METHOD(m_logger);

		try {
			res.setVersion(Response::HTTP_1_1);
			res.setStatusAndReason(Response::HTTP_OK);
			m_route.debug(m_logger, m_params, __FILE__, __LINE__);
			m_route.execute(req, res, m_params, m_userData);
		}
		catch (Poco::Net::NotAuthenticatedException &e) {
			m_logger.log(e, __FILE__, __LINE__);
			res.requireAuthentication(m_name);
		}
		catch (Poco::Exception &e) {
			m_logger.log(e, __FILE__, __LINE__);
			stdout_backtrace();
			log_backtrace(m_logger);
			res.setStatusAndReason(
				Response::HTTP_INTERNAL_SERVER_ERROR);
		}
		catch (std::exception &e) {
			m_logger.critical(e.what(), __FILE__, __LINE__);
			stdout_backtrace();
			log_backtrace(m_logger);
			res.setStatusAndReason(
				Response::HTTP_INTERNAL_SERVER_ERROR);
			res.setReason("Server internal error");
		}
		catch (const char *s) {
			m_logger.critical(s, __FILE__, __LINE__);
			stdout_backtrace();
			log_backtrace(m_logger);
			res.setStatusAndReason(
				Response::HTTP_INTERNAL_SERVER_ERROR);
			res.setReason("Server internal error");
		}
		catch (...) {
			stdout_backtrace();
			log_backtrace(m_logger);
			res.setStatusAndReason(
				Response::HTTP_INTERNAL_SERVER_ERROR);
			res.setReason("Server internal error");
		}

		if (!res.sent())
			res.send();
	}

protected:
	const Route &m_route;
	const typename Route::Params m_params;
	UserData &m_userData;
	const std::string &m_name;
	SessionVerifier m_sessionVerifier;
	Poco::Logger &m_logger;
};

/**
 * Template factory class to create TRestRequestHandlers.
 */
template <typename Request, typename Response, typename UserData = void *>
class TRestRequestHandlerFactory {
public:
	using RequestHandler = TRestRequestHandler
				<Request, Response, UserData>;
	using Route = TRoute<Request, Response, UserData>;
	using RouteVector = typename std::vector<Route>;
	using RouteVectorIterator = typename RouteVector::iterator;
	using Handler = typename Route::Handler;
	using Params = typename Route::Params;

	TRestRequestHandlerFactory(UserData &userData,
			const std::string &name):
		m_noOperation(new Route("*", NULL, false)),
		m_noRoute(new Route("*", NULL, false)),
		m_name(name),
		m_userData(userData),
		m_logger(LOGGER_CLASS(this))
	{
	}

	TRestRequestHandlerFactory(
			Handler noRouteHandler,
			Handler noOperationHandler,
			UserData &userData,
			const std::string &name):
		m_noOperation(new Route("*", noOperationHandler, false)),
		m_noRoute(new Route("*", noRouteHandler, false)),
		m_userData(userData),
		m_name(name),
		m_logger(LOGGER_CLASS(this))
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
		m_noRoute = new Route("*", h);
	}

	void noOperation(Handler h)
	{
		delete m_noOperation;
		m_noOperation = new Route("*", h);
	}

	void POST(const char *pattern, Handler handler, bool secure = true)
	{
		Route route(pattern, handler, secure);
		m_logger.information("register POST %s", route.getURI());
		m_POST.push_back(route);
	}

	void PUT(const char *pattern, Handler handler, bool secure = true)
	{
		Route route(pattern, handler, secure);
		m_logger.information("register PUT %s", route.getURI());
		m_PUT.push_back(route);
	}

	void GET(const char *pattern, Handler handler, bool secure = true)
	{
		Route route(pattern, handler, secure);
		m_logger.information("register GET %s", route.getURI());
		m_GET.push_back(route);
	}

	void DELETE(const char *pattern, Handler handler, bool secure = true)
	{
		Route route(pattern, handler, secure);
		m_logger.information("register DELETE %s", route.getURI());
		m_DELETE.push_back(route);
	}

	void OPTIONS(const char *pattern, Handler handler, bool secure = true)
	{
		Route route(pattern, handler, secure);
		m_logger.information("register OPTIONS %s", route.getURI());
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

		m_logger.debug("no operation %s %s",
				req.getMethod(),
				req.getURI());
		Params noParams;
		RequestHandler *handler = new RequestHandler(
				*m_noOperation, noParams, m_userData, m_name);
		return handler;
	}

protected:

	RequestHandler *routeMatch(const Request &req, const Route &route)
	{
		Params params;

		if (!route.match(req.getURI(), params))
			return NULL;

		m_logger.debug("handle %s %s",
				req.getMethod(),
				req.getURI());

		return new RequestHandler(route, params, m_userData,
				m_name);
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

		m_logger.debug("no route %s %s",
				req.getMethod(),
				req.getURI());
		Params noParams;
		handler = new RequestHandler(*m_noRoute, noParams,
				m_userData, m_name);
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
	UserData &m_userData;
	Poco::Logger &m_logger;
};

}

#endif
