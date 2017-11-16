#include "model/FCMToken.h"

using namespace BeeeOn;
using namespace std;

FCMToken::FCMToken()
{
}

FCMToken::FCMToken(const ID &id):
	Entity<TokenID>(id)
{
}

FCMToken::~FCMToken()
{
}

User FCMToken::user() const
{
	return m_user;
}

void FCMToken::setUser(const User &user)
{
	m_user = user;
}
