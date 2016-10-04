#include <Poco/Logger.h>
#include <Poco/URI.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/StreamCopier.h>

#include "Debug.h"
#include "model/JSONSerializer.h"
#include "provider/GoogleAuthProvider.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Net;
using namespace BeeeOn;

bool GoogleAuthProvider::verifyAuthCode(const string &authCode, AuthResult &info)
{
	string idToken;
	string rawInfo;
	string google_id;
	string email;

	try {
		idToken = requestIdToken(authCode);
		rawInfo = fetchUserInfo(idToken);
	} catch(const Exception &e) {
		m_logger.log(e, __FILE__, __LINE__);
		return false;
	}

	JSONObjectSerializer userInfo(rawInfo);

	if (userInfo.get("sub", google_id))
		info.setProviderID(google_id);
	if (userInfo.get("email", email))
		info.setEmail(email);

	if (email.empty() || google_id.empty())
		return false;

	return true;
}

string GoogleAuthProvider::requestIdToken(const string &authCode)
{
	TRACE_METHOD();

	HTTPSClientSession *session;
	URI uri(m_tokenUrl);

	try {
		initSSL();
		session = new HTTPSClientSession(uri.getHost(), uri.getPort());
	} catch (const Exception &e) {
		m_logger.log(e, __FILE__, __LINE__);
		throw;
	}

	string requestRaw = "code=" + authCode + "&"
		"redirect_uri=postmessage&"
		"client_id=" + m_clientId + "&"
		"client_secret=" + m_clientSecret + "&"
		"scope=&"	// No need to specify, defaults to userinfo.profile,userinfo.email
		"grant_type=authorization_code";

	m_logger.debug("request: " + requestRaw, __FILE__, __LINE__) ;

	HTTPRequest req(HTTPRequest::HTTP_POST,
			uri.getPathAndQuery(),
			HTTPMessage::HTTP_1_1);
	req.setContentType("application/x-www-form-urlencoded");
	req.setContentLength(requestRaw.length());

	session->sendRequest(req) << requestRaw;
	string receiveResponse = handleResponse(*session);

	m_logger.debug("response: " + receiveResponse, __FILE__, __LINE__);

	JSONObjectSerializer userTokens(receiveResponse);
	string idToken;

	// TODO: verify signature of the ID token by Google certificate
	if (userTokens.get("id_token", idToken))
		return idToken;

	m_logger.error("No ID token to obtain user data", __FILE__, __LINE__);

	throw NotAuthenticatedException("Missing id_token field in response.");

}

string GoogleAuthProvider::fetchUserInfo(const string &token)
{
	TRACE_METHOD();

	URI uri(m_tokenInfoUrl + token);
	HTTPSClientSession *session;

	try {
		initSSL();
		session = new HTTPSClientSession(uri.getHost(), uri.getPort());
	} catch (const Exception &e) {
		m_logger.log(e, __FILE__, __LINE__);
		throw;
	}

	HTTPRequest req(HTTPRequest::HTTP_GET,
			uri.getPathAndQuery(),
			HTTPMessage::HTTP_1_1);

	session->sendRequest(req);
	string receiveResponse = handleResponse(*session);

	m_logger.debug("response: " + receiveResponse, __FILE__, __LINE__);

	return receiveResponse;
}

void GoogleAuthProvider::initSSL()
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

string GoogleAuthProvider::convertResponseToString(istream &rs)
{
	string response;
	stringstream ss;
	StreamCopier::copyToString(rs, response);

	return response;
}

string GoogleAuthProvider::handleResponse(HTTPSClientSession &session)
{
	HTTPResponse response;
	istream &rs = session.receiveResponse(response);

	m_logger.debug("response status: " + to_string(response.getStatus())
			+ " " +	response.getReason(), __FILE__, __LINE__);

	string receiveResponse = convertResponseToString(rs);
	m_logger.debug("response: " + receiveResponse, __FILE__, __LINE__);

	if (response.getStatus() != HTTPResponse::HTTP_OK) {
		throw NotAuthenticatedException(
			"failed to retrieve access token from Google APIs");
	}

	return receiveResponse;
}
