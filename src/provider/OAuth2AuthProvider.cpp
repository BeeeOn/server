#include <sstream>

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/NetException.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>

#include "provider/OAuth2AuthProvider.h"
#include "ssl/SSLClient.h"

using namespace std;
using namespace BeeeOn;
using namespace Poco;
using namespace Poco::Net;

OAuth2AuthProvider::OAuth2AuthProvider(const string &name):
		AbstractAuthProvider(name),
		m_sslConfig(0)
{
}

bool OAuth2AuthProvider::authorize(const Credentials &cred, AuthResult &result)
{
	const AuthCodeCredentials &authCodeCredentials =
		static_cast<const AuthCodeCredentials &>(cred);

	result.setProvider(name());
	return verifyAuthCode(authCodeCredentials, result);
}

void OAuth2AuthProvider::initSSL() const
{
	if (m_sslConfig == 0)
		throw IllegalStateException(
				"missing sslConfig, cannot use OAuth");
}

SharedPtr<HTTPSClientSession> OAuth2AuthProvider::connectSecure(
		const std::string &host,
		unsigned int port) const
{
	try {
		initSSL();
		return new HTTPSClientSession(
				host, port, m_sslConfig->context());
	} catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		throw;
	}
}

string OAuth2AuthProvider::handleResponse(HTTPSClientSession &session) const
{
	HTTPResponse response;
	istream &rs = session.receiveResponse(response);

	if (logger().debug()) {
		logger().debug("response status: "
			+ to_string(response.getStatus())
			+ " "
			+ response.getReason(), __FILE__, __LINE__);
	}

	string receiveResponse = convertResponseToString(rs);

	if (logger().debug()) {
		logger().debug("response: "
			+ receiveResponse, __FILE__, __LINE__);
	}

	return receiveResponse;
}

string OAuth2AuthProvider::makeRequest(const string &method, URI &host, HTMLForm &requestForm) const
{
	SharedPtr<HTTPSClientSession>session;
	session = connectSecure(host.getHost(), host.getPort());

	HTTPRequest request(method, host.getPathAndQuery(), HTTPMessage::HTTP_1_1);
	requestForm.prepareSubmit(request);

	ostringstream requestQuery;
	requestForm.write(requestQuery);

	if (logger().debug())
		logger().debug("request: " + requestQuery.str(), __FILE__, __LINE__);

	if (method == HTTPRequest::HTTP_POST)
		session->sendRequest(request) << requestQuery.str();
	else
		session->sendRequest(request);

	return handleResponse(*session);
}

string OAuth2AuthProvider::convertResponseToString(istream &rs) const
{
	string response;
	stringstream ss;
	StreamCopier::copyToString(rs, response);

	return response;
}
