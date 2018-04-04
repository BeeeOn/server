#include "provider/AuthProvider.h"

using namespace std;
using namespace BeeeOn;

ApiKeyCredentials::ApiKeyCredentials(
		const string &provider,
		const TokenID &key):
	Credentials(provider),
	m_key(key)
{
}

const TokenID &ApiKeyCredentials::key() const
{
	return m_key;
}
