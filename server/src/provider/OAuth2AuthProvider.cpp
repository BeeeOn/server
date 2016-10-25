#include <Poco/Exception.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/HTTPSClientSession.h>

#include "provider/OAuth2AuthProvider.h"
#include "Debug.h"

using namespace std;
using namespace BeeeOn;
using namespace Poco;
using namespace Poco::Net;

OAuth2AuthProvider::OAuth2AuthProvider(const string &name):
		AuthCodeAuthProvider(name)
{
	textInjector("client_id",
			(TextSetter) &OAuth2AuthProvider::setClientId);
	textInjector("client_secret",
			(TextSetter) &OAuth2AuthProvider::setClientSecret);
	textInjector("redirect_uri",
			(TextSetter) &OAuth2AuthProvider::setRedirectURI);
}

void OAuth2AuthProvider::initSSL()
{
	TRACE_METHOD();

	initializeSSL();

	/* Handles certificates for HTTPS communication
	 * TODO: accept only google certificate, now accepts all certs!!!
	 */
	SSLManager::InvalidCertificateHandlerPtr ptrHandler (
			new Poco::Net::AcceptCertificateHandler(false));
	const Context::Ptr context = new Context(Context::CLIENT_USE, "");
	SSLManager::instance().initializeClient(0, ptrHandler, context);
}

HTTPSClientSession *OAuth2AuthProvider::connectSecure(
		const std::string &host,
		unsigned int port)
{
	try {
		initSSL();
		return new HTTPSClientSession(host, port);
	} catch (const Exception &e) {
		m_logger.log(e, __FILE__, __LINE__);
		throw;
	}
}
