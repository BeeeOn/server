#include "model/Location.h"

using namespace std;
using namespace BeeeOn;

Location::Location()
{
}

Location::Location(const ID &id):
	Entity(id)
{
}

void Location::setName(const string &name)
{
	m_name = name;
}

const string &Location::name() const
{
	return m_name;
}

void Location::setPlace(const Place &place)
{
	m_place = place;
}

const Place &Location::place() const
{
	return m_place;
}

bool Location::hasPlace() const
{
	return !m_place.id().isNull();
}


