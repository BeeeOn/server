#include <Poco/Logger.h>
#include <Poco/URI.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>

#include "Debug.h"
#include "provider/GoogleAuthProvider.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Net;
using namespace BeeeOn;

bool GoogleAuthProvider::parseIdentity(const std::string &userInfo,
		AuthResult &result)
{
	Parser parser;
	Object::Ptr info = parser.parse(userInfo).extract<Object::Ptr>();

	if (info->has("sub"))
		result.setProviderID(info->getValue<string>("sub"));

	if (info->has("email"))
		result.setEmail(info->getValue<string>("email"));

	if (result.email().empty() || result.providerID().empty())
		return false;

	if (info->has("given_name"))
		result.setFirstName(info->getValue<string>("given_name"));
	if (info->has("family_name"))
		result.setLastName(info->getValue<string>("family_name"));
	if (info->has("picture"))
		result.setPicture(info->getValue<string>("picture"));

	return true;
}

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
		logger().log(e, __FILE__, __LINE__);
		return false;
	}

	info.setAccessToken(idToken);

	return parseIdentity(rawInfo, info);
}

string GoogleAuthProvider::requestIdToken(const string &authCode)
{
	TRACE_METHOD();

	SharedPtr<HTTPSClientSession> session;
	URI uri(m_tokenUrl);

	session = connectSecure(uri.getHost(), uri.getPort());

	string requestRaw = "code=" + authCode + "&"
		"redirect_uri=" + m_redirectURI + "&"
		"client_id=" + m_clientId + "&"
		"client_secret=" + m_clientSecret + "&"
		"scope=&"	// No need to specify, defaults to userinfo.profile,userinfo.email
		"grant_type=authorization_code";

	if (logger().debug())
		logger().debug("request: " + requestRaw, __FILE__, __LINE__);

	HTTPRequest req(HTTPRequest::HTTP_POST,
			uri.getPathAndQuery(),
			HTTPMessage::HTTP_1_1);
	req.setContentType("application/x-www-form-urlencoded");
	req.setContentLength(requestRaw.length());

	session->sendRequest(req) << requestRaw;
	string receiveResponse = handleResponse(*session);

	Parser parser;
	Object::Ptr object = parser.parse(receiveResponse)
			.extract<Object::Ptr>();
	string idToken;

	if (object->has("id_token"))
		return object->getValue<string>("id_token");

	logger().error("No ID token to obtain user data", __FILE__, __LINE__);

	throw NotAuthenticatedException("Missing id_token field in response.");

}

string GoogleAuthProvider::fetchUserInfo(const string &token)
{
	TRACE_METHOD();

	URI uri(m_tokenInfoUrl + token);
	SharedPtr<HTTPSClientSession> session;

	session = connectSecure(uri.getHost(), uri.getPort());

	HTTPRequest req(HTTPRequest::HTTP_GET,
			uri.getPathAndQuery(),
			HTTPMessage::HTTP_1_1);

	session->sendRequest(req);
	return handleResponse(*session);
}
