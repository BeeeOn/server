#include <string>
#include <Poco/XML/XMLWriter.h>
#include <Poco/SAX/AttributesImpl.h>

#include "xmlui/Serializing.h"
#include "model/Gateway.h"
#include "model/Location.h"
#include "model/Device.h"
#include "model/VerifiedIdentity.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

void BeeeOn::XmlUI::serialize(XMLWriter &output, const Gateway &gateway)
{
	AttributesImpl attrs;
	attrs.addAttribute("", "id", "id", "", gateway.id().toString());
	attrs.addAttribute("", "name", "name", "", gateway.name());
	attrs.addAttribute("", "longitude", "longitude", "",
			std::to_string(gateway.longitude()));
	attrs.addAttribute("", "latitude", "latitude", "",
			std::to_string(gateway.latitude()));
	attrs.addAttribute("", "altitude", "altitude", "",
			std::to_string(gateway.altitude()));

	output.emptyElement("", "gate", "gate", attrs);
}

void BeeeOn::XmlUI::serialize(Poco::XML::XMLWriter &output,
		const std::vector<Gateway> &gateways)
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
	attrs.addAttribute("", "type", "type", "", to_string(device.type()));
	attrs.addAttribute("", "locationid", "locationid", "",
			device.location().id().toString());
	attrs.addAttribute("", "gateid", "gateid", "",
			device.gateway().id().toString());
	attrs.addAttribute("", "name", "name", "", device.name());
	attrs.addAttribute("", "status", "status", "",
			device.available()? "available" : "unavailable");
	attrs.addAttribute("", "time", "time", "",
			to_string(device.lastSeen().timestamp().epochTime()));
	attrs.addAttribute("", "init", "init", "",
			device.active()? "1" : "0");
	output.emptyElement("", "device", "device", attrs);
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
