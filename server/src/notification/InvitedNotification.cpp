#include <Poco/Exception.h>

#include "notification/InvitedNotification.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

InvitedNotification::InvitedNotification(
			const Identity &identity,
			const Gateway &gateway,
			const User &sender):
	m_identity(identity),
	m_gateway(gateway),
	m_sender(sender)
{
}

string InvitedNotification::name() const
{
	return "invited";
}

const Identity &InvitedNotification::identity() const
{
	return m_identity;
}

const Gateway &InvitedNotification::gateway() const
{
	return m_gateway;
}

const User &InvitedNotification::sender() const
{
	return m_sender;
}

const InvitedNotification *InvitedNotification::cast(
		const Notification *n)
{
	if (n->name() == "invited")
		return static_cast<const InvitedNotification *>(n);

	return NULL;
}
