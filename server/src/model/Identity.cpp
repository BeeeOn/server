#include "model/Identity.h"

using namespace std;
using namespace BeeeOn;

Identity::Identity()
{
}

Identity::Identity(const ID &id):
	m_id(id)
{
}

Identity::Identity(const std::string &email):
	m_email(email)
{
}

Identity::Identity(const Identity &copy):
	m_id(copy.m_id),
	m_email(copy.m_email)
{
}

Identity::Identity(const ID &id, const Identity &copy):
	m_id(id),
	m_email(copy.m_email)
{
}
