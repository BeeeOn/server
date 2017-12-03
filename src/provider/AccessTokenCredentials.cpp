#include "provider/AuthProvider.h"

using namespace std;
using namespace BeeeOn;

AccessTokenCredentials::AccessTokenCredentials(
		const string &provider,
		const string &accessToken):
	Credentials(provider),
	m_accessToken(accessToken)
{
}

const string &AccessTokenCredentials::accessToken() const
{
	return m_accessToken;
}
