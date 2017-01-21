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
