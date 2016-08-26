#include <iostream>
#include <sstream>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include "ui/Serializing.h"
#include "model/Place.h"

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
