#include "provider/AuthProvider.h"

using namespace std;
using namespace BeeeOn;

AccessTokenCredentials::AccessTokenCredentials(
		const string &provider,
		const TokenID &accessToken):
	Credentials(provider),
	m_accessToken(accessToken)
{
}

const TokenID &AccessTokenCredentials::accessToken() const
{
	return m_accessToken;
}
