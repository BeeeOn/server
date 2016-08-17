#include <Poco/Logger.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/NetException.h>

#include "model/JSONSerializer.h"
#include "ui/Auth.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

void Auth::extractAuthData(
		Logger &logger,
		istream &data,
		string &provider,
		string &authCode)
{
	JSONObjectSerializer jsonData(data);

	if (logger.debug())
		logger.debug(jsonData.toString(), __FILE__, __LINE__);

	if (!jsonData.get("provider", provider)) {
		throw NotAuthenticatedException(
			"Missing field 'provider'", __FILE__, __LINE__);
	}

	if (!jsonData.get("authCode", authCode)) {
		throw NotAuthenticatedException(
			"Missing field 'authCode'", __FILE__, __LINE__);
	}
}

bool Auth::echoSessionIfExists(UIRequest &request, UIResponse &response)
{
	if (!request.hasCredentials())
		return false;

	string scheme;
	string authInfo;

	request.getCredentials(scheme, authInfo);
	response.sendBuffer(authInfo.c_str(), authInfo.size());

	return true;
}

void Auth::handlePost(UIRouteContext &context)
{
	TRACE_FUNC();

	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	AuthService &authService = context.userData().authService();

	try {
		handlePost(logger, request, response, authService);
	} catch (const Exception &e) {
		logger.log(e, __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_UNAUTHORIZED);
		return;
	}
}

void Auth::handlePost(Poco::Logger &logger,
		UIRequest &request,
		UIResponse &response,
		AuthService &authService)
{
	if (echoSessionIfExists(request, response))
		return;

	string provider;
	string authCode;

	extractAuthData(logger, request.stream(), provider, authCode);
	AuthCodeCredentials credentials(provider, authCode);
	string sessionID = authService.login(credentials);

	response.sendBuffer(sessionID.c_str(), sessionID.size());
}

void Auth::handleDelete(UIRouteContext &context)
{
	TRACE_FUNC();

	Logger &logger = context.userData().logger();
	UIRequest &request = context.request();
	UIResponse &response = context.response();
	AuthService &authService = context.userData().authService();

	string scheme;
	SessionID sessionID;

	try {
		request.getCredentials(scheme, sessionID);
	} catch (const Exception &e) {
		logger.log(e.displayText(), __FILE__, __LINE__);
		response.setStatusAndReason(UIResponse::HTTP_UNAUTHORIZED);
		return;
	}

	authService.logout(sessionID);
}
