#include "model/Identity.h"

using namespace std;
using namespace BeeeOn;

Identity::Identity()
{
}

Identity::Identity(const ID &id):
	Entity(id)
{
}

Identity::Identity(const std::string &email):
	m_email(email)
{
}

Identity::Identity(const Identity &copy):
	Entity(copy),
	m_email(copy.m_email)
{
}

Identity::Identity(const ID &id, const Identity &copy):
	Entity(id, copy),
	m_email(copy.m_email)
{
}
