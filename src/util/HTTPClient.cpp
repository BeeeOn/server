#include <sstream>

#include <Poco/Logger.h>

#include "util/HTTPClient.h"
#include "net/HTTPUtil.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

HTTPClient::HTTPClient(
		const string &method,
		const URI &url):
	m_method(method),
	m_url(url),
	m_enforceSecure(true)
{
}

HTTPClient::HTTPClient(
		const string &method,
		const string &url):
	HTTPClient(method, URI(url))
{
}


void HTTPClient::setMethod(const string &method)
{
	m_method = method;
}

void HTTPClient::setURL(const URI &url)
{
	m_url = url;
}

void HTTPClient::setSSLConfig(SSLClient::Ptr config)
{
	m_sslConfig = config;
}

void HTTPClient::setEnforceSecure(bool enforce)
{
	m_enforceSecure = enforce;
}

string HTTPClient::requestBody(HTMLForm &form) const
{
	return responseBody(request(form));
}

string HTTPClient::requestBody(
		const string &pathAndQuery,
		HTMLForm &form) const
{
	return responseBody(request(pathAndQuery, form));
}

HTTPEntireResponse HTTPClient::request(HTMLForm &form) const
{
	return request(m_url.getPathAndQuery(), form);
}

HTTPEntireResponse HTTPClient::request(
		const string &pathAndQuery,
		HTMLForm &form) const
{
	HTTPRequest request(
		m_method,
		pathAndQuery,
		HTTPMessage::HTTP_1_1);
	form.prepareSubmit(request);

	if (m_enforceSecure && m_sslConfig.isNull()) {
		throw IllegalStateException(
			"attempt to create insecure connection to " + request.getURI());
	}

	ostringstream body;
	form.write(body);

	if (logger().trace())
		logger().trace("request: " + body.str(), __FILE__, __LINE__);

	return HTTPUtil::makeRequest(
			request,
			m_url,
			body.str(),
			m_sslConfig);
}

string HTTPClient::responseBody(const HTTPEntireResponse &response) const
{
	if (logger().debug()) {
		logger().debug("response status: "
			+ to_string(response.getStatus())
			+ " "
			+ response.getReason(), __FILE__, __LINE__);
	}

	if (logger().trace()) {
		logger().trace("response: "
			+ response.getBody(), __FILE__, __LINE__);
	}

	return response.getBody();
}
