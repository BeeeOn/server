#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Timestamp.h>
#include <Poco/Net/SocketAddress.h>

#include "model/Gateway.h"

namespace BeeeOn {

class GatewayEvent {
public:
	GatewayEvent(const GatewayID &gatewayID);

	const GatewayID &gatewayID() const;
	const Poco::Timestamp &stamp() const;

	void setSocketAddress(const Poco::Net::SocketAddress &address);
	const Poco::Net::SocketAddress &socketAddress() const;

private:
	Poco::Timestamp m_stamp;
	GatewayID m_gatewayID;
	Poco::Net::SocketAddress m_socketAddress;
};

class GatewayListener {
public:
	typedef Poco::SharedPtr<GatewayListener> Ptr;

	virtual ~GatewayListener();

	virtual void onConnected(const GatewayEvent &e);
	virtual void onReconnected(const GatewayEvent &e);
	virtual void onDisconnected(const GatewayEvent &e);
};

}
