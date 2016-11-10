#include <Poco/Logger.h>
#include <Poco/Notification.h>

#include "notification/NotificationObserver.h"
#include "Debug.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

NotificationObserver::NotificationObserver():
	m_logger(LOGGER_CLASS(this))
{
}

NotificationObserver::NotificationObserver(const NotificationObserver &copy):
	m_logger(copy.m_logger),
	m_accepts(copy.m_accepts)
{
}

NotificationObserver::~NotificationObserver()
{
}

bool NotificationObserver::equals(const AbstractObserver &observer) const
{
	const NotificationObserver *s =
		dynamic_cast<const NotificationObserver *>(&observer);

	if (s == NULL)
		return false;

	return equals(*s);
}

bool NotificationObserver::equals(const NotificationObserver &o) const
{
	return o.m_accepts == m_accepts;
}

bool NotificationObserver::accepts(Notification *n) const
{
	return m_accepts.find(n->name()) != m_accepts.end();
}

void NotificationObserver::disable()
{
	m_accepts.clear(); // TODO: thread safe
}
