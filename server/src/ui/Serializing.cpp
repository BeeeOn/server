#include <iostream>
#include <sstream>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Parser.h>

#include "ui/Serializing.h"
#include "model/Gateway.h"
#include "model/Location.h"

using namespace std;
using namespace Poco::JSON;
using namespace BeeeOn;

ostream &UI::serialize(ostream &o, const Gateway &gateway)
{
	Object::Ptr object(new Object);

	object->set("id", gateway.id().toString());
	object->set("name", gateway.name());
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
	object->set("gateway_id", location.gateway().id().toString());
	object->stringify(o);
	return o;
}
