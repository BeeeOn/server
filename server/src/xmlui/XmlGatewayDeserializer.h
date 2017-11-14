#ifndef BEEEON_Xml_GATEWAY_DESERIALIZER_H
#define BEEEON_Xml_GATEWAY_DESERIALIZER_H

#include <Poco/DOM/Element.h>

#include "l10n/TimeZone.h"
#include "l10n/TimeZoneProvider.h"
#include "service/Deserializer.h"
#include "model/Gateway.h"

namespace BeeeOn {

class XmlGatewayDeserializer : public Deserializer<Gateway> {
public:
	XmlGatewayDeserializer(const Poco::XML::Element &node);

	void setTimeZoneProvider(TimeZoneProvider::Ptr provider);
	TimeZoneProvider::Ptr timeZoneProvider() const;

	void partial(Gateway &gateway) const override;
	void full(Gateway &gateway) const override;

protected:
	TimeZone extractTimeZone(const Poco::XML::Element &node) const;

private:
	const Poco::XML::Element &m_node;
	TimeZoneProvider::Ptr m_timeZoneProvider;
};

}

#endif
