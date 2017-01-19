#ifndef BEEEON_INVITED_NOTIFICATION_H
#define BEEEON_INVITED_NOTIFICATION_H

#include <string>
#include <Poco/Notification.h>

#include "model/Identity.h"
#include "model/Gateway.h"
#include "model/User.h"

namespace BeeeOn {

class InvitedNotification : public Poco::Notification {
public:
	InvitedNotification(
			const Identity &identity,
			const Gateway &gateway,
			const User &sender);

	std::string name() const;
	const Identity &identity() const;
	const Gateway &gateway() const;
	const User &sender() const;

	static const InvitedNotification *cast(const Notification *n);

private:
	Identity m_identity;
	Gateway m_gateway;
	User m_sender;
};

}

#endif
