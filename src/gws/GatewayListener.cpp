#include "gws/GatewayListener.h"

using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

GatewayEvent::GatewayEvent(const GatewayID &id):
	m_gatewayID(id)
{
}

const GatewayID &GatewayEvent::gatewayID() const
{
	return m_gatewayID;
}

const Timestamp &GatewayEvent::stamp() const
{
	return m_stamp;
}

void GatewayEvent::setSocketAddress(const SocketAddress &address)
{
	m_socketAddress = address;
}

const SocketAddress &GatewayEvent::socketAddress() const
{
	return m_socketAddress;
}

GatewayListener::~GatewayListener()
{
}

void GatewayListener::onConnected(const GatewayEvent &e)
{
}

void GatewayListener::onReconnected(const GatewayEvent &e)
{
}

void GatewayListener::onDisconnected(const GatewayEvent &e)
{
}
