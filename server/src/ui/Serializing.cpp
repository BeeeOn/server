#include <iostream>
#include <sstream>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Parser.h>

#include "ui/Serializing.h"
#include "model/Place.h"
#include "model/Gateway.h"
#include "model/Location.h"

using namespace std;
using namespace Poco::JSON;
using namespace BeeeOn;

static Object::Ptr toObject(const Place &place)
{
	Object::Ptr object(new Object);

	object->set("id", place.id().toString());
	object->set("name", place.name());

	return object;
}

std::ostream &BeeeOn::UI::serialize(std::ostream &o, const Place &place)
{
	toObject(place)->stringify(o);
	return o;
}

string BeeeOn::UI::serialize(const Place &place)
{
	ostringstream s;
	serialize(s, place);
	return s.str();
}

ostream &BeeeOn::UI::serialize(ostream &o, const vector<Place> &places)
{
	Array::Ptr array(new Array);

	for (auto it : places) {
	       array->add(toObject(it));
	}

	array->stringify(o);
	return o;
}

string BeeeOn::UI::serialize(const vector<Place> &places)
{
	ostringstream s;
	serialize(s, places);
	return s.str();
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
	object->set("version", gateway.version());
	object->set("ip", gateway.ipAddress().toString());

	if (!gateway.lastChanged().isNull())
		object->set("last_changed", gateway.lastChanged().value().timestamp().epochTime());

	object->stringify(o);
	return o;
}

string UI::serialize(const Gateway &g)
{
	ostringstream s;
	serialize(s, g);
	return s.str();
}

string BeeeOn::UI::serialize(const Location &l)
{
	ostringstream s;
	serialize(s, l);
	return s.str();
}

std::ostream &BeeeOn::UI::serialize(std::ostream &o, const Location &location)
{
	Object::Ptr object(new Object);
	object->set("id", location.id().toString());
	object->set("name", location.name());
	object->set("place_id", location.place().id().toString());
	object->stringify(o);
	return o;
}
