#include <Poco/Logger.h>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "di/Injectable.h"
#include "server/HTTPCustomHeadersFilter.h"

BEEEON_OBJECT_BEGIN(BeeeOn, HTTPCustomHeadersFilter)
BEEEON_OBJECT_CASTABLE(HTTPFilter)
BEEEON_OBJECT_PROPERTY("uriPattern", &HTTPCustomHeadersFilter::setUriPattern)
BEEEON_OBJECT_PROPERTY("headers", &HTTPCustomHeadersFilter::setHeaders)
BEEEON_OBJECT_END(BeeeOn, HTTPCustomHeadersFilter)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

HTTPCustomHeadersFilter::HTTPCustomHeadersFilter():
	m_globPattern(new Glob("*"))
{
}

void HTTPCustomHeadersFilter::setUriPattern(const string &pattern)
{
	m_uriPattern = pattern;
	m_globPattern = new Glob(pattern);
}

void HTTPCustomHeadersFilter::setHeaders(
		const map<string, string> &headers)
{
	m_headers = headers;
}

void HTTPCustomHeadersFilter::apply(
	HTTPServerRequest &req,
	HTTPServerResponse &res)
{
	if (!m_globPattern->match(req.getURI())) {
		if (logger().debug()) {
			logger().debug("no custom headers for "
					+ req.getURI() + " (no match: "
					+ m_uriPattern + ")",
					__FILE__, __LINE__);
		}

		return;
	}

	if (logger().debug()) {
		logger().debug("injecting custom headers for " + req.getURI()
				+ "(" + m_uriPattern + ")",
				__FILE__, __LINE__);
	}

	for (const auto &pair : m_headers)
		res.set(pair.first, pair.second);
}
