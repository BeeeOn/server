#include <string>
#include <Poco/NumberFormatter.h>
#include <Poco/Timestamp.h>
#include <Poco/XML/XMLWriter.h>
#include <Poco/SAX/AttributesImpl.h>

#include "xmlui/Serializing.h"
#include "l10n/TimeZone.h"
#include "model/Gateway.h"
#include "model/LegacyGateway.h"
#include "model/Location.h"
#include "model/DeviceInfo.h"
#include "model/DeviceProperty.h"
#include "model/DeviceWithData.h"
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

	if (gateway.altitude().isNull()) {
		attrs.addAttribute("", "altitude", "altitude", "", "");
	}
	else {
		attrs.addAttribute("", "altitude", "altitude", "",
			std::to_string(gateway.altitude().value()));
	}

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

	const TimeZone &zone = gateway.timeZone();
	const Timestamp now;
	const int offset = zone.utcOffset().totalSeconds()
			+ (zone.appliesDST(now) ? zone.dstOffset().totalSeconds() : 0);

	attrs.addAttribute("", "timezone_name", "timezone_name", "",
			zone.shortName(Locale::system()));
	attrs.addAttribute("", "timezone", "timezone", "", to_string(offset));

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
		const DeviceWithData &device)
{
	AttributesImpl attrs;
	attrs.addAttribute("", "id", "id", "", device.id().toString());
	attrs.addAttribute("", "euid", "euid", "", device.id().toString());
	attrs.addAttribute("", "type", "type", "", device.type()->id().toString());
	attrs.addAttribute("", "locationid", "locationid", "",
			device.location().id().toString());
	attrs.addAttribute("", "gateid", "gateid", "",
			device.gateway().id().toString());

	if (device.name().empty())
		attrs.addAttribute("", "name", "name", "", device.type()->name());
	else
		attrs.addAttribute("", "name", "name", "", device.name());

	const DeviceStatus &status = device.status();

	attrs.addAttribute("", "status", "status", "",
			device.available()? "available" : "unavailable");
	attrs.addAttribute("", "time", "time", "",
			to_string(status.lastSeen().epochTime()));
	attrs.addAttribute("", "involved", "involved", "",
			to_string(status.firstSeen().epochTime()));
	attrs.addAttribute("", "init", "init", "",
			status.active()? "1" : "0");

	string state;

	switch (status.state()) {
	case DeviceStatus::STATE_INACTIVE:
		state = "inactive";
		break;
	case DeviceStatus::STATE_INACTIVE_PENDING:
		state = "inactive-pending";
		break;
	case DeviceStatus::STATE_ACTIVE:
		state = "active";
		break;
	case DeviceStatus::STATE_ACTIVE_PENDING:
		state = "active-pending";
		break;
	default:
		throw IllegalStateException(
			"invalid state: " + to_string(status.state()));
	}

	attrs.addAttribute("", "state", "state", "", state);
	attrs.addAttribute("", "last-changed", "last-changed", "",
			to_string(status.lastChanged().epochTime()));

	const Poco::SharedPtr<DeviceInfo> info = device.type();

	attrs.addAttribute("", "type_name", "type_name", "", info->name());
	attrs.addAttribute("", "vendor", "vendor", "", info->vendor());

	output.startElement("", "device", "device", attrs);

	const auto &values = device.values();

	for (auto module : *info) {
		AttributesImpl attrs;

		attrs.addAttribute("", "id", "id", "", module.id().toString());
		// FIXME: just copy device status for now
		attrs.addAttribute("", "status", "status", "",
				device.available()? "available" : "unavailable");

		string value = "NaN";

		const unsigned int id = module.id();

		if (id < values.size()) {
			const ValueAt &current = values.at(id);

			if (current.isValid())
				value = NumberFormatter::format(current.value());

			attrs.addAttribute("", "value", "value", "", value);
			attrs.addAttribute("", "valid", "valid", "",
					NumberFormatter::format(current.isValid()));
			attrs.addAttribute("", "at", "at", "",
					NumberFormatter::format(current.at().epochTime()));
		}

		attrs.addAttribute("", "type", "type", "", to_string(int(module.type()->id())));

		if (!module.type()->unit().empty())
			attrs.addAttribute("", "type-unit", "type-unit", "", module.type()->unit());

		if (module.type()->range().isValid()) {
			const auto range = module.type()->range();

			if (range.hasMin()) {
				attrs.addAttribute("", "type-range-min", "type-range-min", "",
						to_string(range.min()));
			}
			if (range.hasMax()) {
				attrs.addAttribute("", "type-range-max",
					"type-range-max", "", to_string(range.max()));
			}
			if (range.hasStep()) {
				attrs.addAttribute("", "type-range-step",
					"type-range-step", "", to_string(range.step()));
			}
		}

		if (!module.type()->values().empty()) {
			string values;

			for (const auto &pair : module.type()->values()) {
				values += to_string(pair.first);
				values += ":";
				values += pair.second;
				values += ";";
			}

			attrs.addAttribute("", "type-enum-values",
					"type-enum-values", "", values);
		}
		else if (!module.subtype().isNull() && !module.subtype()->values().empty()) {
			string values;

			for (const auto &pair : module.subtype()->values()) {
				values += to_string(pair.first);
				values += ":";
				values += pair.second;
				values += ";";
			}

			attrs.addAttribute("", "type-enum-values",
					"type-enum-values", "", values);
		}

		if (!module.name().empty())
			attrs.addAttribute("", "name", "name", "", module.name());
		if (!module.group().empty())
			attrs.addAttribute("", "group", "group", "", module.group());

		if (module.isControllable())
			attrs.addAttribute("", "actuator", "actuator", "", "yes");
		
		output.emptyElement("", "module", "module", attrs);
	}

	output.endElement("", "device", "device");
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const std::vector<DeviceWithData> &devices)
{
	for (auto device : devices)
		serialize(output, device);
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const std::list<DeviceWithData> &devices)
{
	for (auto device : devices)
		serialize(output, device);
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const DecryptedDeviceProperty &property,
		const Device &device)
{
	AttributesImpl attrs;

	attrs.addAttribute("", "euid", "euid", "",
			device.id().toString());
	attrs.addAttribute("", "gateid", "gateid",  "",
			device.gateway().id().toString());
	attrs.addAttribute("", "parameterkey", "parameterkey",  "",
			property.key().toString());

	string value;

	switch (property.key().raw()) {
	case DevicePropertyKey::KEY_IP_ADDRESS:
		value = property.asIPAddress().toString();
		break;
	case DevicePropertyKey::KEY_FIRMWARE:
		value = property.asFirmware();
		break;
	case DevicePropertyKey::KEY_PASSWORD:
		value = property.asPassword();
		break;
	default:
		break;
	}

	attrs.addAttribute("", "parametervalue", "parametervalue",  "", value);

	output.emptyElement("", "device", "device", attrs);
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
