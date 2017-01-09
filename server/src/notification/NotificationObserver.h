#ifndef BEEEON_NOTIFICATION_OBSERVER_H
#define BEEEON_NOTIFICATION_OBSERVER_H

#include <set>
#include <Poco/AbstractObserver.h>

#include "util/Loggable.h"

namespace BeeeOn {

class NotificationObserver : public Poco::AbstractObserver,
		public Loggable {
public:
	NotificationObserver();
	NotificationObserver(const NotificationObserver &copy);
	virtual ~NotificationObserver();

	bool equals(const Poco::AbstractObserver &observer) const override;
	virtual bool equals(const NotificationObserver &o) const;

	bool accepts(Poco::Notification *n) const override;
	void disable() override;

protected:
	std::set<std::string> m_accepts;
};

}

#endif
