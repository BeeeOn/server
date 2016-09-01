#include <iostream>
#include <sstream>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include "ui/Serializing.h"
#include "model/Place.h"
#include "model/Gateway.h"

using namespace std;
using namespace Poco::JSON;
using namespace BeeeOn;

std::ostream &BeeeOn::UI::serialize(std::ostream &o, const Place &place)
{
	Object::Ptr object(new Object);
	object->set("id", place.id().toString());
	object->set("name", place.name());
	object->stringify(o);
	return o;
}

string BeeeOn::UI::serialize(const Place &place)
{
	ostringstream s;
	serialize(s, place);
	return s.str();
}

istream &BeeeOn::UI::deserialize(istream &i, Place &place)
{
	Parser parser;
	Object::Ptr object = parser.parse(i).extract<Object::Ptr>();
	place.setName(object->getValue<string>("name"));

	return i;
}

ostream &UI::serialize(ostream &o, const Gateway &gateway)
{
	Object::Ptr object(new Object);

	object->set("id", gateway.id().toString());
	object->set("name", gateway.name());
	object->set("place_id", gateway.place().id().toString());
	object->set("altitude", gateway.altitude());
	object->set("latitude", gateway.latitude());
	object->set("longitude", gateway.longitude());

	object->stringify(o);
	return o;
}

string UI::serialize(const Gateway &g)
{
	ostringstream s;
	serialize(s, g);
	return s.str();
}

istream &UI::deserialize(istream &i, Gateway &gateway)
{
	Parser parser;

	Object::Ptr object = parser.parse(i).extract<Object::Ptr>();

	if (object->has("name"))
		gateway.setName(object->getValue<string>("name"));

	if (object->has("altitude"))
		gateway.setAltitude(object->getValue<double>("altitude"));

	if (object->has("latitude"))
		gateway.setLatitude(object->getValue<double>("latitude"));

	if (object->has("longitude"))
		gateway.setLongitude(object->getValue<double>("longitude"));

	return i;
}
