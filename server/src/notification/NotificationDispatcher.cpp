#include <Poco/AbstractObserver.h>

#include "notification/NotificationDispatcher.h"
#include "notification/FirstLoginNotification.h"
#include "notification/InvitedNotification.h"
#include "model/Identity.h"
#include "model/VerifiedIdentity.h"
#include "model/Gateway.h"
#include "model/User.h"
#include "Debug.h"

BEEEON_OBJECT(NotificationDispatcher, BeeeOn::NotificationDispatcher)

using namespace Poco;
using namespace BeeeOn;

NotificationDispatcher::NotificationDispatcher()
{
	injector("observer", &NotificationDispatcher::addObserver);
}

NotificationDispatcher::~NotificationDispatcher()
{
}

void NotificationDispatcher::addObserver(AbstractObserver *o)
{
	m_center.addObserver(*o);
}

void NotificationDispatcher::notifyFirstLogin(
		const VerifiedIdentity &identity)
{
	m_center.postNotification(new FirstLoginNotification(identity));
}

void NotificationDispatcher::notifyInvited(
		const Identity &identity,
		const Gateway &gateway,
		const User &by)
{
	m_center.postNotification(new InvitedNotification(
		identity, gateway, by
	));
}
