#include <sstream>

#include <Poco/Exception.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/NetException.h>
#include <Poco/StreamCopier.h>

#include "provider/OAuth2AuthProvider.h"
#include "ssl/SSLClient.h"
#include "Debug.h"

using namespace std;
using namespace BeeeOn;
using namespace Poco;
using namespace Poco::Net;

OAuth2AuthProvider::OAuth2AuthProvider(const string &name):
		AuthCodeAuthProvider(name),
		m_sslConfig(0)
{
}

void OAuth2AuthProvider::initSSL()
{
	if (m_sslConfig == 0)
		throw IllegalStateException(
				"missing sslConfig, cannot use OAuth");
}

SharedPtr<HTTPSClientSession> OAuth2AuthProvider::connectSecure(
		const std::string &host,
		unsigned int port)
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

string OAuth2AuthProvider::handleResponse(HTTPSClientSession &session)
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

string OAuth2AuthProvider::convertResponseToString(istream &rs)
{
	string response;
	stringstream ss;
	StreamCopier::copyToString(rs, response);

	return response;
}
