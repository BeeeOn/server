#ifndef BEEEON_NOTIFICATION_OBSERVER_H
#define BEEEON_NOTIFICATION_OBSERVER_H

#include <set>
#include <Poco/AbstractObserver.h>

namespace Poco {

class Logger;

}

namespace BeeeOn {

class NotificationObserver : public Poco::AbstractObserver {
public:
	NotificationObserver();
	NotificationObserver(const NotificationObserver &copy);
	virtual ~NotificationObserver();

	bool equals(const Poco::AbstractObserver &observer) const override;
	virtual bool equals(const NotificationObserver &o) const;

	bool accepts(Poco::Notification *n) const override;
	void disable() override;

protected:
	Poco::Logger &m_logger;
	std::set<std::string> m_accepts;
};

}

#endif
