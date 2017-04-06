#include <string>
#include <Poco/XML/XMLWriter.h>
#include <Poco/SAX/AttributesImpl.h>

#include "xmlui/Serializing.h"
#include "model/Gateway.h"
#include "model/LegacyGateway.h"
#include "model/Location.h"
#include "model/Device.h"
#include "model/DeviceInfo.h"
#include "model/RoleInGateway.h"
#include "model/LegacyRoleInGateway.h"
#include "model/VerifiedIdentity.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

static void prepare(AttributesImpl &attrs, const Gateway &gateway)
{
	attrs.addAttribute("", "id", "id", "", gateway.id().toString());
	attrs.addAttribute("", "name", "name", "", gateway.name());
	attrs.addAttribute("", "longitude", "longitude", "",
			std::to_string(gateway.longitude()));
	attrs.addAttribute("", "latitude", "latitude", "",
			std::to_string(gateway.latitude()));
	attrs.addAttribute("", "altitude", "altitude", "",
			std::to_string(gateway.altitude()));
	attrs.addAttribute("", "version", "version", "", gateway.version());
	attrs.addAttribute("", "ip", "ip", "", gateway.ipAddress().toString());
}

void BeeeOn::XmlUI::serialize(XMLWriter &output, const Gateway &gateway)
{
	AttributesImpl attrs;
	prepare(attrs, gateway);
	output.emptyElement("", "gate", "gate", attrs);
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const std::vector<Gateway> &gateways)
{
	for (auto gateway : gateways)
		serialize(output, gateway);
}

void BeeeOn::XmlUI::serialize(XMLWriter &output, const LegacyGateway &gateway)
{
	AttributesImpl attrs;
	prepare(attrs, gateway);

	attrs.addAttribute("", "owner", "owner", "",
			gateway.owner().fullName());
	attrs.addAttribute("", "permission", "permission", "",
			gateway.accessLevel().toString());
	attrs.addAttribute("", "devices", "devices", "",
			to_string(gateway.deviceCount()));
	attrs.addAttribute("", "users", "users", "",
			to_string(gateway.userCount()));
	attrs.addAttribute("", "timezone", "timezone", "", "0");
	attrs.addAttribute("", "status", "status", "", "available");

	output.emptyElement("", "gate", "gate", attrs);
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const std::vector<LegacyGateway> &gateways)
{
	for (auto gateway : gateways)
		serialize(output, gateway);
}

void BeeeOn::XmlUI::serialize(XMLWriter &output, const Location &location)
{
	AttributesImpl attrs;
	attrs.addAttribute("", "id", "id", "", location.id().toString());
	attrs.addAttribute("", "locationid", "locationid", "",
			location.id().toString());
	attrs.addAttribute("", "name", "name", "", location.name());
	attrs.addAttribute("", "type", "type", "", "0"); // FIXME

	output.emptyElement("", "location", "location", attrs);
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const std::vector<Location> &locations)
{
	for (auto location : locations)
		serialize(output, location);
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const Device &device)
{
	AttributesImpl attrs;
	attrs.addAttribute("", "id", "id", "", device.id().toString());
	attrs.addAttribute("", "euid", "euid", "", device.id().toString());
	attrs.addAttribute("", "type", "type", "", device.type()->id().toString());
	attrs.addAttribute("", "locationid", "locationid", "",
			device.location().id().toString());
	attrs.addAttribute("", "gateid", "gateid", "",
			device.gateway().id().toString());
	attrs.addAttribute("", "name", "name", "", device.name());
	attrs.addAttribute("", "status", "status", "",
			device.available()? "available" : "unavailable");
	attrs.addAttribute("", "time", "time", "",
			to_string(device.lastSeen().timestamp().epochTime()));
	attrs.addAttribute("", "involved", "involved", "",
			to_string(device.firstSeen().timestamp().epochTime()));
	attrs.addAttribute("", "init", "init", "",
			device.active()? "1" : "0");

	const Poco::SharedPtr<DeviceInfo> info = device.type();

	attrs.addAttribute("", "displayName", "displayName", "", info->displayName());
	attrs.addAttribute("", "name", "name", "", info->name());
	attrs.addAttribute("", "vendor", "vendor", "", info->vendor());

	output.startElement("", "device", "device", attrs);

	for (auto module : *info) {
		AttributesImpl attrs;

		attrs.addAttribute("", "id", "id", "", module.id().toString());
		// FIXME: just copy device status for now
		attrs.addAttribute("", "status", "status", "",
				device.available()? "available" : "unavailable");
		// FIXME: no values implemented, send neutral 0
		attrs.addAttribute("", "value", "value", "", "0");

		attrs.addAttribute("", "type", "type", "", module.type()->id().toString());

		if (!module.name().empty())
			attrs.addAttribute("", "name", "name", "", module.name());
		if (!module.group().empty())
			attrs.addAttribute("", "group", "group", "", module.group());

		output.emptyElement("", "module", "module", attrs);
	}

	output.endElement("", "device", "device");
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const std::vector<Device> &devices)
{
	for (auto device : devices)
		serialize(output, device);
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const std::list<Device> &devices)
{
	for (auto device : devices)
		serialize(output, device);
}

static void prepare(AttributesImpl &attrs, const RoleInGateway &role)
{
	attrs.addAttribute("", "id", "id", "", role.id().toString());
	attrs.addAttribute("", "email", "email", "",
			role.identity().email());
	attrs.addAttribute("", "level", "level", "",
			role.level().toString());
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const RoleInGateway &role)
{
	AttributesImpl attrs;
	prepare(attrs, role);
	output.emptyElement("", "user", "user", attrs);
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const std::vector<RoleInGateway> &roles)
{
	for (auto role : roles)
		serialize(output, role);
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const LegacyRoleInGateway &role)
{
	AttributesImpl attrs;
	prepare(attrs, role);

	if (role.isOwner()) {
		attrs.addAttribute("", "permission", "permission",
				"", "owner");
	}
	else {
		attrs.addAttribute("", "permission", "permission",
				"", role.level().toString());
	}

	attrs.addAttribute("", "gender", "gender", "", "unknown");
	attrs.addAttribute("", "name", "name", "", role.firstName());
	attrs.addAttribute("", "surname", "surname", "", role.lastName());
	attrs.addAttribute("", "imgurl", "imgurl", "", role.picture().toString());

	output.emptyElement("", "user", "user", attrs);
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const std::vector<LegacyRoleInGateway> &roles)
{
	for (auto role : roles)
		serialize(output, role);
}

void BeeeOn::XmlUI::serializeMyself(
		Poco::XML::XMLWriter &output,
		const VerifiedIdentity &identity)
{
	const User &user = identity.user();

	AttributesImpl attrs;
	attrs.addAttribute("", "id", "id", "", identity.id().toString());
	attrs.addAttribute("", "name", "name", "", user.firstName());
	attrs.addAttribute("", "first_name", "first_name", "",
			user.firstName());
	attrs.addAttribute("", "surname", "surname", "", user.lastName());
	attrs.addAttribute("", "last_name", "last_name", "",
			user.lastName());
	attrs.addAttribute("", "gender", "gender", "", "unknown");
	attrs.addAttribute("", "email", "email", "", identity.email());
	attrs.addAttribute("", "imgurl", "imgurl", "",
			identity.picture().toString());
	output.emptyElement("", "user", "user", attrs);
}
