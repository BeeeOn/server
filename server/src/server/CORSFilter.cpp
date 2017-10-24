#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/NetException.h>

#include "di/Injectable.h"
#include "server/CORSFilter.h"

BEEEON_OBJECT_BEGIN(BeeeOn, CORSFilter)
BEEEON_OBJECT_CASTABLE(HTTPFilter)
BEEEON_OBJECT_NUMBER("requireOrigin", &CORSFilter::setRequireOrigin)
BEEEON_OBJECT_LIST("allowedOrigins", &CORSFilter::setAllowedOrigins)
BEEEON_OBJECT_LIST("allowedMethods", &CORSFilter::setAllowedMethods)
BEEEON_OBJECT_LIST("allowedHeaders", &CORSFilter::setAllowedHeaders)
BEEEON_OBJECT_LIST("simpleHeaders", &CORSFilter::setSimpleHeaders)
BEEEON_OBJECT_LIST("exposeHeaders", &CORSFilter::setExposeHeaders)
BEEEON_OBJECT_TIME("preflightMaxAge", &CORSFilter::setPreflightMaxAge)
BEEEON_OBJECT_NUMBER("allowCredentials", &CORSFilter::setAllowCredentials)
BEEEON_OBJECT_END(BeeeOn, CORSFilter)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

CORSFilter::CORSFilter():
	m_requireOrigin(true),
	m_allowCredentials(false),
	m_preflightMaxAge(10 * Timespan::SECONDS)
{
	m_simpleHeaders.emplace("Accept");
	m_simpleHeaders.emplace("Accept-Encoding");
	m_simpleHeaders.emplace("Accept-Language");
	m_simpleHeaders.emplace("Connection");
	m_simpleHeaders.emplace("Content-Language");
	m_simpleHeaders.emplace("Content-Length");
	m_simpleHeaders.emplace("DPR");
	m_simpleHeaders.emplace("Downlink");
	m_simpleHeaders.emplace("Host");
	m_simpleHeaders.emplace("Last-Event-ID");
	m_simpleHeaders.emplace("Origin");
	m_simpleHeaders.emplace("Save-Data");
	m_simpleHeaders.emplace("User-Agent");
	m_simpleHeaders.emplace("Viewport-Width");
	m_simpleHeaders.emplace("Width");
}

void CORSFilter::setRequireOrigin(bool require)
{
	m_requireOrigin = require;
}

void CORSFilter::setAllowedOrigins(const list<string> &origins)
{
	m_allowedOrigins.clear();

	for (const auto &s : origins)
		m_allowedOrigins.emplace(s);
}

void CORSFilter::setAllowedMethods(const list<string> &methods)
{
	m_allowedMethods.clear();

	for (const auto &s : methods)
		m_allowedMethods.emplace(s);
}

void CORSFilter::setAllowedHeaders(const list<string> &headers)
{
	m_allowedHeaders.clear();

	for (const auto &s : headers)
		m_allowedHeaders.emplace(s);
}

void CORSFilter::setSimpleHeaders(const list<string> &headers)
{
	m_simpleHeaders.clear();

	for (const auto &s : headers)
		m_simpleHeaders.emplace(s);
}

void CORSFilter::setAllowCredentials(bool allow)
{
	m_allowCredentials = allow;
}

void CORSFilter::setExposeHeaders(const list<string> &headers)
{
	m_exposeHeaders.clear();

	for (const auto &s : headers)
		m_exposeHeaders.emplace(s);
}

void CORSFilter::setPreflightMaxAge(const Timespan &maxAge)
{
	m_preflightMaxAge = maxAge;
}

void CORSFilter::apply(HTTPServerRequest &req, HTTPServerResponse &res)
{
	if (req.getMethod() == "OPTIONS")
		handlePreflight(req, res);
	else if (m_requireOrigin || req.has("Origin"))
		filterRequest(req, res);
}

void CORSFilter::handlePreflight(const HTTPServerRequest &req, HTTPServerResponse &res) const
{
	if (logger().debug()) {
		logger().debug("handling preflight request "
			+ req.getMethod() + " " + req.getURI(),
			__FILE__, __LINE__);
	}

	if (!req.has("Origin"))
		throw InvalidArgumentException("missing Origin header");

	const string &origin = req.get("Origin");

	if (m_allowedOrigins.empty())
		res.set("Access-Control-Allow-Origin", origin);
	else if (m_allowedOrigins.find(origin) != m_allowedOrigins.end())
		res.set("Access-Control-Allow-Origin", origin);
	else
		throw InvalidArgumentException("disallowed Origin: " + origin);

	if (!m_allowedMethods.empty())
		res.set("Access-Control-Allow-Methods", formatSet(m_allowedMethods, ", "));

	if (!m_allowedHeaders.empty())
		res.set("Access-Control-Allow-Headers", formatSet(m_allowedHeaders, ", "));

	if (m_allowCredentials)
		res.set("Access-Control-Allow-Credentials", "true");

	if (!m_exposeHeaders.empty())
		res.set("Access-Control-Expose-Headers", formatSet(m_exposeHeaders, ", "));

	if (m_preflightMaxAge >= 0)
		res.set("Access-Control-Max-Age", to_string(m_preflightMaxAge.totalSeconds()));

	res.send();
}

void CORSFilter::filterRequest(const HTTPServerRequest &req, HTTPServerResponse &res) const
{
	if (logger().debug()) {
		logger().debug("filtering request "
			+ req.getMethod() + " " + req.getURI()
			+ " for CORS",
			__FILE__, __LINE__);
	}

	if (!req.has("Origin"))
		throw InvalidArgumentException("missing required Origin header");

	const string &origin = req.get("Origin");

	if (!m_allowedOrigins.empty()) { // otherwise, allow any ('*')
		if (m_allowedOrigins.find(origin) == m_allowedOrigins.end())
			throw InvalidArgumentException("disallowed Origin: " + origin);
	}

	res.set("Access-Control-Allow-Origin", origin);

	if (!m_allowedMethods.empty()) { // otherwise, allow any
		const string &method = req.getMethod();

		if (m_allowedMethods.find(method) == m_allowedMethods.end())
			throw InvalidArgumentException("disallowed method: " + method);
	}

	for (const auto &pair : req) {
		if (m_simpleHeaders.find(pair.first) != m_simpleHeaders.end())
			continue;

		if (m_allowedHeaders.find(pair.first) == m_allowedHeaders.end())
			throw InvalidArgumentException("disallowed header: " + pair.first);
	}

	if (m_allowCredentials)
		res.set("Access-Control-Allow-Credentials", "true");
}

string CORSFilter::formatSet(const set<string> &s,
			const string &separator) const
{
	string result;

	for (auto it = s.begin(); it != s.end(); ++it) {
		if (it != s.begin())
			result += separator;
		
		result += *it;
	}

	return result;
}
