#include "di/Injectable.h"
#include "util/Sanitize.h"
#include "xmlui/AuthXmlHandler.h"

using namespace std;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;

AuthXmlHandler::AuthXmlHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input,
		BeeeOn::AuthService &service):
	AbstractXmlHandler("accounts", socket, input),
	m_authService(service)
{
}

void AuthXmlHandler::handleInputImpl()
{
	Element *root = m_input->documentElement();
	const string &type = root->getAttribute("type");

	if (type == "logout") {
		m_authService.logout(root->getAttribute("sessionid"));
		resultSuccess();
	}
	else if (type == "login") {
		Element *provider = root->getChildElement("provider");
		if (provider == NULL) {
			resultInvalidInput();
			return;
		}

		AuthCodeCredentials credentials(
			Sanitize::strict(provider->getAttribute("name")),
			provider->getAttribute("authCode")
		);

		const Session::Ptr session = m_authService.login(credentials);

		if (session.isNull()) {
			resultNotAuthenticated();
			return;
		}

		resultDataStart();

		AttributesImpl attrs;
		attrs.addAttribute("", "id", "id", "", session->sessionID());
		m_output.emptyElement("", "session", "session", attrs);

		resultDataEnd();
	}
	else
		resultInvalidInput();
}

AuthXmlHandlerResolver::AuthXmlHandlerResolver():
	AbstractXmlHandlerResolver("accounts")
{
}

bool AuthXmlHandlerResolver::canHandle(const Element &root)
{
	if (!AbstractXmlHandlerResolver::canHandle(root))
		return false;

	if (root.getAttribute("type") == "login")
		return true;

	if (root.getAttribute("type") == "logout")
		return true;

	return false;
}

XmlRequestHandler *AuthXmlHandlerResolver::createHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	return new AuthXmlHandler(socket, input, *m_authService);
}

BEEEON_OBJECT_BEGIN(BeeeOn, XmlUI, AuthXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(AbstractXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(XmlRequestHandlerResolver)
BEEEON_OBJECT_REF("authService", &AuthXmlHandlerResolver::setAuthService)
BEEEON_OBJECT_END(BeeeOn, XmlUI, AuthXmlHandlerResolver)
