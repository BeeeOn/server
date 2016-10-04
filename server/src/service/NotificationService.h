#ifndef BEEEON_NOTIFICATION_SERVICE_H
#define BEEEON_NOTIFICATION_SERVICE_H

#include <Poco/Logger.h>

#include "di/InjectorTarget.h"
#include "model/VerifiedIdentity.h"

namespace BeeeOn {

class NotificationService {
public:
	NotificationService();

	virtual void notifyFirstLogin(const VerifiedIdentity &identity) = 0;

protected:
	Poco::Logger &m_logger;
};

class FakeNotificationService : public AbstractInjectorTarget,
		public NotificationService {
public:
	void notifyFirstLogin(const VerifiedIdentity &identity);
};

}

#endif
