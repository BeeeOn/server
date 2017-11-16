#ifndef BEEEON_XML_LOCATION_DESERIALIZER_H
#define BEEEON_XML_LOCATION_DESERIALIZER_H

#include <Poco/DOM/Element.h>

#include "service/Deserializer.h"
#include "model/Location.h"

namespace BeeeOn {

class XmlLocationDeserializer : public Deserializer<Location> {
public:
	XmlLocationDeserializer(const Poco::XML::Element &node);

	void partial(Location &location) const override;
	void full(Location &location) const override;

private:
	const Poco::XML::Element &m_node;
};

}

#endif
