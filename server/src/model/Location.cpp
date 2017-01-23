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

void Location::setGateway(const Gateway &gateway)
{
	m_gateway = gateway;
}

const Gateway &Location::gateway() const
{
	return m_gateway;
}
