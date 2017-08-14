#include <vector>

#include "di/Injectable.h"
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
		Session::Ptr session,
		IdentityService &identityService):
	SessionXmlHandler("accounts", socket, input, session),
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
	Single<VerifiedIdentity> input(identity);
	User user(session()->userID());
	input.setUser(user);

	m_identityService.fetch(input);

	resultDataStart();
	serializeMyself(m_output, identity);
	resultDataEnd();
}

ProfileXmlHandlerResolver::ProfileXmlHandlerResolver():
	SessionXmlHandlerResolver("accounts")
{
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
	Session::Ptr session = lookupSession(*m_sessionManager, input);
	return new ProfileXmlHandler(
			socket, input, session,
			*m_identityService);
}

BEEEON_OBJECT_BEGIN(BeeeOn, XmlUI, ProfileXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(SessionXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(AbstractXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(XmlRequestHandlerResolver)
BEEEON_OBJECT_REF("identityService", &ProfileXmlHandlerResolver::setIdentityService)
BEEEON_OBJECT_REF("sessionManager", &ProfileXmlHandlerResolver::setSessionManager)
BEEEON_OBJECT_END(BeeeOn, XmlUI, ProfileXmlHandlerResolver)
