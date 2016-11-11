#include <Poco/AbstractObserver.h>

#include "notification/NotificationDispatcher.h"
#include "notification/FirstLoginNotification.h"
#include "model/VerifiedIdentity.h"
#include "Debug.h"

BEEEON_OBJECT(NotificationDispatcher, BeeeOn::NotificationDispatcher)

using namespace Poco;
using namespace BeeeOn;

NotificationDispatcher::NotificationDispatcher():
	m_logger(LOGGER_CLASS(this))
{
	injector<NotificationDispatcher, AbstractObserver>(
		"observer",
		&NotificationDispatcher::addObserver
	);
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
