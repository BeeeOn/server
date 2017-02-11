#ifndef BEEEON_NOTIFICATION_DISPATCHER_H
#define BEEEON_NOTIFICATION_DISPATCHER_H

#include <Poco/NotificationCenter.h>

#include "di/AbstractInjectorTarget.h"
#include "model/VerifiedIdentity.h"

namespace Poco {

class AbstractObserver;

}

namespace BeeeOn {

class Identity;
class Gateway;
class User;

class NotificationDispatcher : public AbstractInjectorTarget {
public:
	NotificationDispatcher();
	virtual ~NotificationDispatcher();

	void addObserver(Poco::AbstractObserver *o);

	void notifyFirstLogin(const VerifiedIdentity &identity);
	void notifyInvited(const Identity &identity, const Gateway &gateway, const User &by);

protected:
	Poco::NotificationCenter m_center;
};

}

#endif
