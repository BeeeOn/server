#include <vector>

#include "model/Identity.h"
#include "model/Gateway.h"
#include "model/RoleInGateway.h"
#include "model/LegacyRoleInGateway.h"
#include "server/AccessLevel.h"
#include "server/SessionManager.h"
#include "service/RoleService.h"
#include "xmlui/Serializing.h"
#include "xmlui/RoleXmlHandler.h"
#include "xmlui/XmlRoleInGatewayDeserializer.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

RoleXmlHandler::RoleXmlHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input,
		ExpirableSession::Ptr session,
		RoleService &roleService):
	AbstractXmlHandler("gateusers", socket, input, session),
	m_roleService(roleService)
{
}

void RoleXmlHandler::handleInputImpl()
{
	if (!requireSession())
		return;

	Element *root = m_input->documentElement();
	const string &type = root->getAttribute("type");
	const string &gateid = root->getAttribute("gateid");

	if (type == "getall") {
		handleGetAll(gateid);
		return;
	}

	Element *userNode = root->getChildElement("user");
	if (userNode == NULL) {
		resultInvalidInput();
		return;
	}

	if (type == "invite")
		handleInvite(gateid, userNode);
	else if (type == "remove")
		handleRemove(gateid, userNode);
	else if (type == "updateaccess")
		handleUpdateAccess(gateid, userNode);
	else
		resultInvalidInput();
}

void RoleXmlHandler::handleInvite(const string &gateid, Element *userNode)
{
	Gateway gateway(GatewayID::parse(gateid));

	const string email(userNode->getAttribute("email"));
	const string perm(userNode->getAttribute("permission"));

	Identity identity;
	identity.setEmail(email);
	AccessLevel as(AccessLevel::parse(perm));

	Relation<Identity, Gateway> input(identity, gateway);
	User user(session()->userID());
	input.setUser(user);

	m_roleService.inviteIdentity(input, as);
	resultSuccess();
}

void RoleXmlHandler::handleGetAll(const string &gateid)
{
	Gateway gateway(GatewayID::parse(gateid));
	vector<LegacyRoleInGateway> roles;

	Relation<vector<LegacyRoleInGateway>, Gateway> input(roles, gateway);
	User user(session()->userID());
	input.setUser(user);

	m_roleService.list(input);

	resultDataStart();
	serialize(m_output, roles);
	resultDataEnd();
}

void RoleXmlHandler::handleRemove(const string &gateid, Element *userNode)
{
	Gateway gateway(GatewayID::parse(gateid));
	RoleInGateway role(RoleInGatewayID::parse(userNode->getAttribute("id")));

	Single<RoleInGateway> input(role);
	User user(session()->userID());
	input.setUser(user);

	m_roleService.remove(input);
	resultSuccess();
}

void RoleXmlHandler::handleUpdateAccess(const string &gateid, Element *userNode)
{
	GatewayID::parse(gateid); // not needed, only check validity

	RoleInGateway role(RoleInGatewayID::parse(userNode->getAttribute("id")));
	XmlRoleInGatewayDeserializer deserializer(*userNode);

	SingleWithData<RoleInGateway> input(role, deserializer);
	User user(session()->userID());
	input.setUser(user);

	m_roleService.update(input);
	resultSuccess();
}

RoleXmlHandlerResolver::RoleXmlHandlerResolver():
	AbstractXmlHandlerResolver("gateusers")
{
	injector<RoleXmlHandlerResolver, RoleService>(
		"roleService",
		&RoleXmlHandlerResolver::setRoleService
	);
	injector<RoleXmlHandlerResolver, SessionManager>(
		"sessionManager",
		&RoleXmlHandlerResolver::setSessionManager
	);
}

bool RoleXmlHandlerResolver::canHandle(const Element &root)
{
	if (!AbstractXmlHandlerResolver::canHandle(root))
		return false;

	const string &type = root.getAttribute("type");

	if (type == "invite")
		return true;
	if (type == "getall")
		return true;
	if (type == "remove")
		return true;
	if (type == "updateaccess")
		return true;

	return false;
}

XmlRequestHandler *RoleXmlHandlerResolver::createHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	ExpirableSession::Ptr session = lookupSession(
			*m_sessionManager, input);
	return new RoleXmlHandler(socket, input,
			session, *m_roleService);
}

BEEEON_OBJECT(RoleXmlHandlerResolver,
		BeeeOn::XmlUI::RoleXmlHandlerResolver)
