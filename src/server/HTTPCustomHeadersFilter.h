#pragma once

#include <map>
#include <string>

#include <Poco/Glob.h>
#include <Poco/SharedPtr.h>

#include "server/HTTPFilter.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief HTTPCustomHeadersFilter injects headers into all
 * responses based on the preset URI pattern. The pattern is
 * used to match the request URI by the glob algorithm.
 *
 * @see https://pocoproject.org/docs/Poco.Glob.html
 */
class HTTPCustomHeadersFilter : public HTTPFilter, Loggable {
public:
	/**
	 * @brief Create filter applying to URI pattern <code>*</code>
	 * which means *all*.
	 */
	HTTPCustomHeadersFilter();

	/**
	 * @brief Set URI pattern to select request URIs to which
	 * responses apply headers to.
	 */
	void setUriPattern(const std::string &pattern);

	/**
	 *  @briefSet map of headers to be injected into responses.
	 */
	void setHeaders(const std::map<std::string, std::string> &headers);

	/**
	 * @brief Apply the configured headers to the given response
	 * if the request URI matches the preconfigured URI pattern.
	 */
	void apply(
		Poco::Net::HTTPServerRequest &req,
		Poco::Net::HTTPServerResponse &res) override;

private:
	std::map<std::string, std::string> m_headers;
	std::string m_uriPattern;
	Poco::SharedPtr<Poco::Glob> m_globPattern;
};

}
