#include <vector>

#include "service/IdentityService.h"
#include "xmlui/ProfileXmlHandler.h"
#include "xmlui/Serializing.h"
#include "model/VerifiedIdentity.h"
#include "model/Identity.h"
#include "model/User.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

ProfileXmlHandler::ProfileXmlHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input,
		ExpirableSession::Ptr session,
		IdentityService &identityService):
	AbstractXmlHandler("accounts", socket, input, session),
	m_identityService(identityService)
{
}

void ProfileXmlHandler::handleInputImpl()
{
	if (!requireSession())
		return;

	Element *root = m_input->documentElement();
	const string &type = root->getAttribute("type");

	if (type == "getmyprofile")
		handleMyProfile();
	else
		resultInvalidInput();
}

void ProfileXmlHandler::handleMyProfile()
{
	VerifiedIdentity identity(session()->identityID());
	m_identityService.fetch(identity);

	resultDataStart();
	serializeMyself(m_output, identity);
	resultDataEnd();
}

ProfileXmlHandlerResolver::ProfileXmlHandlerResolver():
	AbstractXmlHandlerResolver("accounts")
{
	injector("identityService", &ProfileXmlHandlerResolver::setIdentityService);
	injector("sessionManager", &ProfileXmlHandlerResolver::setSessionManager);
}

bool ProfileXmlHandlerResolver::canHandle(
		const Element &root)
{
	const string &type = root.getAttribute("type");

	if (type == "getmyprofile")
		return true;

	return false;
}

XmlRequestHandler *ProfileXmlHandlerResolver::createHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	ExpirableSession::Ptr session = lookupSession(
			*m_sessionManager, input);
	return new ProfileXmlHandler(
			socket, input, session,
			*m_identityService);
}

BEEEON_OBJECT(BeeeOn, XmlUI, ProfileXmlHandlerResolver)

