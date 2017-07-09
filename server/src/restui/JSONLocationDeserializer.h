#ifndef BEEEON_JSON_LOCATION_DESERIALIZER_H
#define BEEEON_JSON_LOCATION_DESERIALIZER_H

#include <Poco/JSON/Object.h>

#include "service/Deserializer.h"
#include "model/Location.h"

namespace BeeeOn {

class JSONLocationDeserializer : public Deserializer<Location> {
public:
	JSONLocationDeserializer(const Poco::JSON::Object::Ptr object);

	void partial(Location &location) const override;
	void full(Location &location) const override;

private:
	const Poco::JSON::Object::Ptr m_object;
};

}

#endif
