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
			provider->getAttribute("name"),
			provider->getAttribute("authCode")
		);

		const ExpirableSession::Ptr session =
			m_authService.login(credentials);

		if (session.isNull()) {
			resultNotAuthenticated();
			return;
		}
	
		AttributesImpl attrs;
		attrs.addAttribute("", "sessionid", "sessionid", "",
				session->sessionID());
		resultSimple(attrs, "ok");
	}
	else
		resultInvalidInput();
}

AuthXmlHandlerResolver::AuthXmlHandlerResolver():
	AbstractXmlHandlerResolver("accounts")
{
	injector<AuthXmlHandlerResolver, AuthService>("authService",
			&AuthXmlHandlerResolver::setAuthService);
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

BEEEON_OBJECT(AuthXmlHandlerResolver, BeeeOn::XmlUI::AuthXmlHandlerResolver)
