#ifndef BEEEON_FIRST_LOGIN_NOTIFICATION_H
#define BEEEON_FIRST_LOGIN_NOTIFICATION_H

#include <string>
#include <Poco/Notification.h>

#include "model/VerifiedIdentity.h"

namespace BeeeOn {

class FirstLoginNotification : public Poco::Notification {
public:
	FirstLoginNotification(const VerifiedIdentity &identity);

	std::string name() const;
	const VerifiedIdentity &identity() const;

	static const FirstLoginNotification *cast(const Notification *n);

private:
	VerifiedIdentity m_identity;
};

}

#endif
