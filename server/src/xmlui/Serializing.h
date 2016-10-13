#ifndef BEEEON_XMLUI_SERIALIZING_H
#define BEEEON_XMLUI_SERIALIZING_H

#include <vector>

namespace Poco {
namespace XML {
	class XMLWriter;
	class Element;
}
}

namespace BeeeOn {

class Gateway;
class Location;

namespace XmlUI {

void serialize(Poco::XML::XMLWriter &output, const Gateway &gateway);
void serialize(Poco::XML::XMLWriter &output,
		const std::vector<Gateway> &gateways);

void serialize(Poco::XML::XMLWriter &output, const Location &location);
void serialize(Poco::XML::XMLWriter &output,
		const std::vector<Location> &locations);

}
}

#endif

