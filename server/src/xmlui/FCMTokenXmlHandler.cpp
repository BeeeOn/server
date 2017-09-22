#include <vector>

#include "di/Injectable.h"
#include "xmlui/FCMTokenXmlHandler.h"
#include "xmlui/Serializing.h"
#include "model/FCMToken.h"
#include "model/VerifiedIdentity.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, XmlUI, FCMTokenXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(XmlRequestHandlerResolver)
BEEEON_OBJECT_REF("fcmTokenService", &FCMTokenXmlHandlerResolver::setFCMTokenService)
BEEEON_OBJECT_REF("sessionManager", &FCMTokenXmlHandlerResolver::setSessionManager)
BEEEON_OBJECT_END(BeeeOn, XmlUI, FCMTokenXmlHandlerResolver)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

FCMTokenXmlHandler::FCMTokenXmlHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input,
		Session::Ptr session,
		FCMTokenService &tokenService):
	SessionXmlHandler("notifications", socket, input, session),
	m_tokenService(tokenService)
{
}

void FCMTokenXmlHandler::handleInputImpl()
{
	if (!requireSession())
		return;

	Element *root = m_input->documentElement();
	const string &type = root->getAttribute("type");

	Element *tokenNode = root->getChildElement("service");
	if (tokenNode == NULL) {
		resultInvalidInput();
	} else {
		string serviceName = tokenNode->getAttribute("name");
		if (serviceName != "fcm") {
			resultInvalidInput();
		}
	}

	if (type == "register")
		handleRegister(tokenNode);
	else if (type == "unregister")
		handleUnregister(tokenNode);
	else
		resultInvalidInput();
}

void FCMTokenXmlHandler::handleRegister(Element *tokenNode)
{
	FCMToken token(FCMTokenID::parse(Sanitize::token(tokenNode->getAttribute("id"))));
	User user(session()->userID());
	token.setUser(user);
	Single<FCMToken> input(token);
	input.setUser(user);

	m_tokenService.registerToken(input);

	resultSuccess();
}

void FCMTokenXmlHandler::handleUnregister(Element *tokenNode)
{
	FCMToken token(FCMTokenID::parse(Sanitize::token(tokenNode->getAttribute("id"))));
	User user(session()->userID());
	token.setUser(user);
	Single<FCMToken> input(token);
	input.setUser(user);

	m_tokenService.unregisterToken(input);

	resultSuccess();
}

FCMTokenXmlHandlerResolver::FCMTokenXmlHandlerResolver():
	SessionXmlHandlerResolver("notifications")
{
}

void FCMTokenXmlHandlerResolver::setFCMTokenService(FCMTokenService::Ptr service)
{
	m_tokenService = service;
}

void FCMTokenXmlHandlerResolver::setSessionManager(SessionManager::Ptr manager)
{
	m_sessionManager = manager;
}

bool FCMTokenXmlHandlerResolver::canHandle(
		const Element &root)
{
	if (!SessionXmlHandlerResolver::canHandle(root))
		return false;

	const string &type = root.getAttribute("type");

	if (type == "register")
		return true;
	if (type == "unregister")
		return true;

	return false;
}

XmlRequestHandler *FCMTokenXmlHandlerResolver::createHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	Session::Ptr session = lookupSession(*m_sessionManager, input);
	return new FCMTokenXmlHandler(
			socket, input, session,
			*m_tokenService);
}
