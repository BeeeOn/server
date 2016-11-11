#include <Poco/Notification.h>

#include "notification/LoggingObserver.h"
#include "notification/FirstLoginNotification.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT(LoggingObserver, BeeeOn::LoggingObserver)

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

static void notify(Logger &logger, const FirstLoginNotification *n)
{
	if (n == NULL)
		return;

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

void LoggingObserver::notify(Notification *n) const
{
	::notify(m_logger, FirstLoginNotification::cast(n));
}

AbstractObserver *LoggingObserver::clone() const
{
	return new LoggingObserver();
}
