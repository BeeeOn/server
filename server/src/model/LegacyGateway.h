#ifndef BEEEON_LEGACY_GATEWay_H
#define BEEEON_LEGACY_GATEWay_H

#include "model/Gateway.h"
#include "model/User.h"
#include "server/AccessLevel.h"

namespace BeeeOn {

/**
 * The purpose of this class is to support the XML-UI API
 * that assumes we have more information then in the plain
 * Gateway.
 */
class LegacyGateway : public Gateway {
public:
	LegacyGateway();
	LegacyGateway(const GatewayID &id);
	LegacyGateway(const LegacyGateway &copy);
	LegacyGateway(const GatewayID &id, const LegacyGateway &copy);
	LegacyGateway(const Gateway &copy);

	void setOwner(const User &owner);
	const User &owner() const;

	void setAccessLevel(const AccessLevel &level);
	const AccessLevel &accessLevel() const;

	void setDeviceCount(unsigned int count);
	unsigned int deviceCount() const;

	void setUserCount(unsigned int count);
	unsigned int userCount() const;

private:
	User m_owner;
	AccessLevel m_accessLevel;
	unsigned int m_deviceCount;
	unsigned int m_userCount;
};

}

#endif
