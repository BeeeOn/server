#ifndef BEEEON_LEGACY_ROLE_IN_GATEWAY_H
#define BEEEON_LEGACY_ROLE_IN_GATEWAY_H

#include <string>

#include <Poco/URI.h>

#include "model/RoleInGateway.h"

namespace BeeeOn {

class LegacyRoleInGateway : public RoleInGateway {
public:
	LegacyRoleInGateway();
	LegacyRoleInGateway(const RoleInGatewayID &id);

	void setOwner(bool owner);
	bool isOwner() const;

	void setFirstName(const std::string &name);
	const std::string &firstName() const;

	void setLastName(const std::string &name);
	const std::string &lastName() const;

	void setPicture(const Poco::URI &picture);
	const Poco::URI &picture() const;

private:
	bool m_owner;
	std::string m_firstName;
	std::string m_lastName;
	Poco::URI m_picture;
};

}

#endif
