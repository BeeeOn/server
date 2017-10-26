#ifndef BEEEON_CORS_FILTER_H
#define BEEEON_CORS_FILTER_H

#include <list>
#include <set>
#include <string>

#include <Poco/Timespan.h>

#include "server/HTTPFilter.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief General CORS HTTP filter. It can handle the preflight requests
 * and check all other requests to meet the given requirements. It works
 * globally for all requests and their potential handlers.
 */
class CORSFilter : public HTTPFilter, Loggable {
public:
	CORSFilter();

	/**
	 * Make the Origin HTTP header mandatory for all requests
	 * or allow to handle requests without that header.
	 *
	 * If disabled, the Origin header is required only for the
	 * CORS preflight requests. Also, the CORS is not applied
	 * for requests without the Origin header.
	 */
	void setRequireOrigin(bool require);

	/**
	 * Set origins that are allowed during cross-domain requests.
	 * If it is empty then the injected CORS response header
	 * Control-Access-Allow-Origin will be '*'.
	 */
	void setAllowedOrigins(const std::list<std::string> &origins);

	/**
	 * Set methods that are allowed during cross-domain requests.
	 * If it is empty, all methods are allowed and the CORS response
	 * header Control-Access-Allow-Methods is not injected.
	 */
	void setAllowedMethods(const std::list<std::string> &methods);

	/**
	 * Set headers that are allowed during cross-domain requests.
	 */
	void setAllowedHeaders(const std::list<std::string> &headers);

	/**
	 * Override simple headers that are allowed during cross-domain requests.
	 */
	void setSimpleHeaders(const std::list<std::string> &headers);

	/**
	 * Set whether credentials is allowed via cross-domain requests.
	 * If true, the header Access-Control-Allow-Credentials: true is
	 * injected.
	 */
	void setAllowCredentials(bool credentials);

	/**
	 * Set headers that are allowed to be exposed to the browser client code
	 * during cross-domain requests.
	 */
	void setExposeHeaders(const std::list<std::string> &headers);

	/**
	 * Set the time period for how long the preflight requests should be cached
	 * by the browser.
	 */
	void setPreflightMaxAge(const Poco::Timespan &maxAge);

	/**
	 * Handle the preflight OPTIONS requests and check normal
	 * requests according to the set policy.
	 * For non-preflight requests, generate CORS response headers.
	 */
	void apply(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &res) override;

protected:
	/**
	 * Handle the preflight OPTIONS request and generates HTTP response.
	 */
	void handlePreflight(const Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &res) const;

	/**
	 * Filter all requests accoroding to CORS depending on the m_requireOrigin setting.
	 * If a request does not meet the CORS requirements, an exception is thrown to
	 * disallow further request processing.
	 */
	void filterRequest(const Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &res) const;

	std::string formatSet(const std::set<std::string> &s,
			const std::string &separator) const;

private:
	bool m_requireOrigin;
	std::set<std::string> m_allowedOrigins;
	std::set<std::string> m_allowedMethods;
	std::set<std::string> m_allowedHeaders;
	std::set<std::string> m_simpleHeaders;
	bool m_allowCredentials;
	std::set<std::string> m_exposeHeaders;
	Poco::Timespan m_preflightMaxAge;
};

}

#endif
