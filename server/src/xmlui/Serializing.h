#ifndef BEEEON_XMLUI_SERIALIZING_H
#define BEEEON_XMLUI_SERIALIZING_H

#include <list>
#include <vector>

namespace Poco {
namespace XML {
	class XMLWriter;
	class Element;
}
}

namespace BeeeOn {

class Gateway;
class LegacyGateway;
class Location;
class Device;
class VerifiedIdentity;

namespace XmlUI {

void serialize(Poco::XML::XMLWriter &output, const Gateway &gateway);
void serialize(Poco::XML::XMLWriter &output,
		const std::vector<Gateway> &gateways);

void serialize(Poco::XML::XMLWriter &output, const LegacyGateway &gateway);
void serialize(Poco::XML::XMLWriter &output,
		const std::vector<LegacyGateway> &gateways);

void serialize(Poco::XML::XMLWriter &output, const Location &location);
void serialize(Poco::XML::XMLWriter &output,
		const std::vector<Location> &locations);

void serialize(Poco::XML::XMLWriter &output, const Device &device);
void serialize(Poco::XML::XMLWriter &output,
		const std::vector<Device> &devices);
void serialize(Poco::XML::XMLWriter &output,
		const std::list<Device> &devices);

void serializeMyself(Poco::XML::XMLWriter &output,
		const VerifiedIdentity &identity);

}
}

#endif

