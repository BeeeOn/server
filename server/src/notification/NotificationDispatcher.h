#ifndef BEEEON_NOTIFICATION_DISPATCHER_H
#define BEEEON_NOTIFICATION_DISPATCHER_H

#include <Poco/Logger.h>
#include <Poco/NotificationCenter.h>

#include "di/InjectorTarget.h"
#include "model/VerifiedIdentity.h"

namespace Poco {

class AbstractObserver;

}

namespace BeeeOn {

class NotificationDispatcher : public AbstractInjectorTarget {
public:
	NotificationDispatcher();
	virtual ~NotificationDispatcher();

	void addObserver(Poco::AbstractObserver *o);

	void notifyFirstLogin(const VerifiedIdentity &identity);

protected:
	Poco::Logger &m_logger;
	Poco::NotificationCenter m_center;
};

}

#endif
