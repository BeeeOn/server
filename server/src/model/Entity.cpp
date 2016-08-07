#include <sstream>
#include <typeinfo>
#include <Poco/Exception.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Dynamic/Var.h>

#include "Entity.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace BeeeOn;

Entity::Entity(const string &data)
{
	Parser parser;
	Dynamic::Var var = parser.parse(data);
	constructor(var);
}

void Entity::constructor(const Var &data)
{
	if (data.type() != typeid(Poco::JSON::Object::Ptr))
		throw InvalidArgumentException("expected a JSON object");

	m_data = data.extract<Poco::JSON::Object::Ptr>();
}

void Entity::print(ostream &o) const
{
	m_data->stringify(o);
}

const string Entity::toString() const
{
	ostringstream s;
	print(s);
	return s.str();
}

EntityCollection::EntityCollection(const string &data)
{
	Parser parser;
	Dynamic::Var parsed = parser.parse(data);
	constructor(parsed);
}

void EntityCollection::constructor(const Var &data)
{
	if (data.type() != typeid(Poco::JSON::Array::Ptr))
		throw InvalidArgumentException("expected a JSON array");

	m_data = data.extract<Poco::JSON::Array::Ptr>();
}

void EntityCollection::print(ostream &o) const
{
	m_data->stringify(o);
}

const string EntityCollection::toString() const
{
	ostringstream s;
	print(s);
	return s.str();
}
