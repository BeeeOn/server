#pragma once

#include <istream>
#include <Poco/JSON/Object.h>

#include "l10n/TimeZone.h"
#include "l10n/TimeZoneProvider.h"
#include "service/Deserializer.h"
#include "model/Gateway.h"

namespace BeeeOn {

class JSONGatewayDeserializer : public Deserializer<Gateway> {
public:
	JSONGatewayDeserializer(const std::string &input);
	JSONGatewayDeserializer(std::istream &input);
	JSONGatewayDeserializer(const Poco::JSON::Object::Ptr data);

	void setTimeZoneProvider(TimeZoneProvider::Ptr provider);
	TimeZoneProvider::Ptr timeZoneProvider() const;

	void partial(Gateway &gateway) const override;
	void full(Gateway &gateway) const override;

protected:
	TimeZone extractTimeZone(const Poco::JSON::Object::Ptr data) const;

private:
	Poco::JSON::Object::Ptr m_data;
	TimeZoneProvider::Ptr m_timeZoneProvider;
};

}
