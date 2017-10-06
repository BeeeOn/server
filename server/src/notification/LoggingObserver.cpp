#include <Poco/Logger.h>
#include <Poco/Notification.h>

#include "di/Injectable.h"
#include "notification/LoggingObserver.h"
#include "notification/FirstLoginNotification.h"
#include "notification/InvitedNotification.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, LoggingObserver)
BEEEON_OBJECT_CASTABLE(NotificationObserver)
BEEEON_OBJECT_CASTABLE(AbstractObserver)
BEEEON_OBJECT_END(BeeeOn, LoggingObserver)

LoggingObserver::LoggingObserver()
{
}

LoggingObserver::~LoggingObserver()
{
}

bool LoggingObserver::equals(const NotificationObserver &o) const
{
	const LoggingObserver *co = dynamic_cast<const LoggingObserver *>(&o);
	if (co != NULL)
		return NotificationObserver::equals(o);

	return false;
}

static void logNotification(Logger &logger, const FirstLoginNotification *n)
{
	const VerifiedIdentity &identity = n->identity();
	const User &user = identity.user();

	logger.information("first login of "
		+ identity.identity().email()
		+ ", "
		+ identity.provider()
		+ ", "
		+ identity.id().toString()
		+ ", "
		+ user.firstName()
		+ " "
		+ user.lastName(),
		__FILE__, __LINE__);
}

static void logNotification(Logger &logger, const InvitedNotification *n)
{
	const Identity &identity = n->identity();
	const Gateway &gateway = n->gateway();
	const User &sender = n->sender();

	logger.information("inviting "
		+ identity.email()
		+ " to access gateway "
		+ gateway.id().toString()
		+ " by "
		+ sender.id().toString());
}

template <typename T>
static void notify(Logger &logger, const T *n)
{
	if (n == NULL)
		return;

	logNotification(logger, n);
}

void LoggingObserver::notify(Notification *n) const
{
	::notify(logger(), FirstLoginNotification::cast(n));
	::notify(logger(), InvitedNotification::cast(n));
}

AbstractObserver *LoggingObserver::clone() const
{
	return new LoggingObserver();
}
