#pragma once

#include <Poco/DateTime.h>
#include <Poco/Nullable.h>
#include <Poco/Net/IPAddress.h>

#include "model/GatewayID.h"

namespace BeeeOn {

class GatewayStatus {
public:
	void setLastChanged(const Poco::Nullable<Poco::DateTime> &at);
	Poco::Nullable<Poco::DateTime> lastChanged() const;

	void setVersion(const std::string &version);
	std::string version() const;

	void setIPAddress(const std::string &ipAddress);
	void setIPAddress(const Poco::Net::IPAddress &ipAddress);
	Poco::Net::IPAddress ipAddress() const;

private:
	Poco::Nullable<Poco::DateTime> m_lastChanged;
	std::string m_version;
	Poco::Net::IPAddress m_ipAddress;
};

}
