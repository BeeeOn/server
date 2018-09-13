#pragma once

#include <istream>
#include <Poco/JSON/Object.h>

#include "service/Deserializer.h"
#include "model/Location.h"

namespace BeeeOn {

class JSONLocationDeserializer : public Deserializer<Location> {
public:
	JSONLocationDeserializer(const std::string &input);
	JSONLocationDeserializer(std::istream &input);
	JSONLocationDeserializer(const Poco::JSON::Object::Ptr data);

	void partial(Location &location) const override;
	void full(Location &location) const override;

private:
	Poco::JSON::Object::Ptr m_data;
};

}
