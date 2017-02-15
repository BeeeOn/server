#ifndef BEEEON_CONSOLE_OBSERVER_H
#define BEEEON_CONSOLE_OBSERVER_H

#include "notification/NotificationObserver.h"

namespace BeeeOn {

class LoggingObserver :
	public NotificationObserver {
public:
	LoggingObserver();
	~LoggingObserver();

	bool equals(const NotificationObserver &o) const override;
	void notify(Poco::Notification *n) const override;
	Poco::AbstractObserver *clone() const override;
};

}

#endif
