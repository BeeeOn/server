#ifndef BEEEON_JSON_GATEWAY_DESERIALIZER_H
#define BEEEON_JSON_GATEWAY_DESERIALIZER_H

#include <istream>
#include <Poco/JSON/Object.h>

#include "service/Deserializer.h"
#include "model/Place.h"

namespace BeeeOn {

class JSONPlaceDeserializer : public Deserializer<Place> {
public:
	JSONPlaceDeserializer(const std::string &input);
	JSONPlaceDeserializer(std::istream &input);
	JSONPlaceDeserializer(const Poco::JSON::Object::Ptr data);

	void partial(Place &place) const override;
	void full(Place &place) const override;

private:
	Poco::JSON::Object::Ptr m_data;
};

}

#endif
