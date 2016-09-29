#ifndef BEEEON_ROUTE_H
#define BEEEON_ROUTE_H

#include <vector>
#include <map>
#include <functional>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/Logger.h>

namespace BeeeOn {

template <typename Request, typename Response, typename UserData = void *>
class TRouteContext;

/**
 * RESTful route representation.
 * Any kind of Request, Response pair can be used. We just need
 * to be able to take a reference of them (early binding).
 * Each resolution of a route takes a UserData argument to have
 * a pointer to some request specific data.
 *
 * Examples:
 *   /devices/getAll   -> void getAll(context)
 *   /user/get/:userId -> void getUser(context)  [userId -> <value>]
 *
 * Simple wildcards are possible. A rule "*" match any request.
 * Otherwise, every path fragment can be subtituted by "*" to match all
 * such fragments.
 */
template <typename Request, typename Response, typename UserData = void *>
class TRoute {
public:
	/**
	 * Params data structure is currently represented by std::map.
	 */
	typedef std::map<std::string, std::string> Params;

	/**
	 * Related TRouteContext can be simply called Context.
	 */
	typedef TRouteContext<Request, Response, UserData> Context;

	/**
	 * Function for handling a routing request. The request
	 * data is represented by the given context.
	 */
	typedef std::function<void (Context &context)> Handler;

	std::string getURI() const
	{
		return m_uri.getPath();
	}

	TRoute(std::string const &uri, Handler h, bool secure = true):
		m_handler(h),
		m_uri(uri),
		m_secure(secure)
	{
		m_uri.getPathSegments(m_path);
	}

	TRoute(Poco::URI &uri, Handler h, bool secure = true):
		m_handler(h),
		m_uri(uri),
		m_secure(secure)
	{
		m_uri.getPathSegments(m_path);
	}

	TRoute(const TRoute &copy):
		m_handler(copy.m_handler),
		m_uri(copy.m_uri),
		m_path(copy.m_path),
		m_secure(copy.m_secure)
	{
	}

	~TRoute() {}

	/**
	 * @see match(Poco::URI, Params)
	 */
	bool match(std::string const &uri, Params &p) const
	{
		Poco::URI parsed(uri);
		return match(parsed, p);
	}

	/**
	 * Match the given URI for this route. If the route matches
	 * it returns true and the extracted parameters are inserted
	 * into the given params.
	 */
	bool match(Poco::URI &uri, Params &p) const
	{
		// match wildcard "*"
		if (m_path.size() == 1 && m_uri.getPath() == "*")
			return true;

		if (m_uri.empty() || uri.empty())
			return false;

		size_t count;
		std::vector<std::string> path;
		uri.getPathSegments(path);

		count = path.size();

		if (count != m_path.size())
			return false;

		Params tmp;

		for (unsigned int i = 0; i < count; ++i) {
			// match partial wildcard
			if (m_path[i] == "*")
				continue;
			else if (m_path[i].at(0) == ':')
				tmp.insert(make_pair(m_path[i].substr(1), path[i]));
			else if (m_path[i] != path[i])
				return false;
		}

		p = tmp;
		return true;
	}

	/**
	 * Return true when this Route is considered secure. A secure
	 * Route requires authentication.
	 */
	bool isSecure() const
	{
		return m_secure;
	}

	/**
	 * Execute the route's handler. The handler is called with
	 * the given request, response, params and userData.
	 *
	 * @throws Poco::NullPointerException when no handler is set
	 *	for this route
	 */
	void execute(Request &request,
			Response &response,
			const Params &params,
			UserData &userData) const
	{
		Context context(request, response, params, userData);
		execute(context);
	}

	/**
	 * Execute the route's handler with the given context.
	 *
	 * @throws Poco::NullPointerException when no handler is set
	 *	for this route
	 */
	void execute(Context &context) const
	{
		if (m_handler == NULL)
			throw Poco::NullPointerException("no handler for this route");
		m_handler(context);
	}

	/**
	 * Debug the Route instance with the given set of extracted
	 * params. Reports the route into the given logger on DEBUG level.
	 */
	void debug(Poco::Logger &logger, const Params &p,
			const char *file, size_t line) const
	{
		if (!logger.debug())
			return;

		std::string message = "route " + getURI() + " -> [";

		Params::const_iterator it = p.begin();
		if (it != p.end()) {
			message += it->first + " = " + it->second;
			++it;
		}

		for (; it != p.end(); ++it)
			message += ", " + it->first + " = " + it->second;
		message += "]";

		logger.debug(message, file, line);
	}

private:
	Handler m_handler;
	Poco::URI m_uri;
	std::vector<std::string> m_path;
	bool m_secure;
};

/**
 * Represents a context of a route resolution. It provides
 * access to the Request and Response instances, the params
 * extracted from URL (URI) and the userData specific to
 * the request.
 */
template <typename Request, typename Response, typename UserData>
class TRouteContext {
public:
	typedef typename TRoute<Request, Response>::Params Params;

	TRouteContext(Request &request,
			Response &response,
			const Params &params,
			UserData &userData):
		m_request(request),
		m_response(response),
		m_params(params),
		m_userData(userData)
	{
	}

	Request &request() const
	{
		return m_request;
	}

	Response &response() const
	{
		return m_response;
	}

	const Params &params() const
	{
		return m_params;
	}

	UserData &userData() const
	{
		return m_userData;
	}

private:
	Request &m_request;
	Response &m_response;
	const Params &m_params;
	UserData &m_userData;
};

}

#endif
