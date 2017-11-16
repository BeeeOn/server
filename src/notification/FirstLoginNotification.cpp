#include <Poco/Exception.h>

#include "notification/FirstLoginNotification.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

FirstLoginNotification::FirstLoginNotification(
		const VerifiedIdentity &identity):
	m_identity(identity)
{
}

string FirstLoginNotification::name() const
{
	return "first-login";
}

const VerifiedIdentity &FirstLoginNotification::identity() const
{
	return m_identity;
}

const FirstLoginNotification *FirstLoginNotification::cast(
		const Notification *n)
{
	if (n->name() == "first-login")
		return static_cast<const FirstLoginNotification *>(n);

	return NULL;
}
