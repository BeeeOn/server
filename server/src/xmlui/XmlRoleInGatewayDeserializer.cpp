#include <Poco/Exception.h>
#include <Poco/DOM/Element.h>

#include "server/AccessLevel.h"
#include "xmlui/XmlRoleInGatewayDeserializer.h"

using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

XmlRoleInGatewayDeserializer::XmlRoleInGatewayDeserializer(
		const Element &node):
	m_node(node)
{
}

void XmlRoleInGatewayDeserializer::partial(RoleInGateway &role) const
{
	if (m_node.hasAttribute("permission"))
		role.setLevel(AccessLevel::parse(m_node.getAttribute("permission")));
}

void XmlRoleInGatewayDeserializer::full(RoleInGateway &role) const
{
	if (!m_node.hasAttribute("permission"))
		throw InvalidArgumentException("missing permission for role");

	role.setLevel(AccessLevel::parse(m_node.getAttribute("permission")));
}
