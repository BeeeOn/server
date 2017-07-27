#include "xmlui/SessionXmlHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

SessionXmlHandler::SessionXmlHandler(
		const string &ns,
		const StreamSocket &socket,
		const AutoPtr<Document> input,
		Session::Ptr session):
	AbstractXmlHandler(ns, socket, input),
	m_session(session)
{
}

bool SessionXmlHandler::requireSession()
{
	if (!hasSession()) {
		resultNotAuthenticated();
		return false;
	}

	return true;
}

bool SessionXmlHandler::hasSession() const
{
	return !m_session.isNull();
}

const Session::Ptr SessionXmlHandler::session() const
{
	return m_session;
}

SessionXmlHandlerResolver::SessionXmlHandlerResolver(const string &ns):
	AbstractXmlHandlerResolver(ns)
{
}

Session::Ptr SessionXmlHandlerResolver::lookupSession(
		SessionManager &sessionManager,
		const AutoPtr<Document> input)
{
	const Element *root = input->documentElement();
	const string &sessionId = root->getAttribute("sessionid");
	ExpirableSession::Ptr session;

	if (!sessionManager.lookup(sessionId, session))
		return NULL;

	return session;
}
