#ifndef BEEEON_LEGACY_GATEWAY_RPC_H
#define BEEEON_LEGACY_GATEWAY_RPC_H

#include <Poco/LogStream.h>
#include <Poco/Mutex.h>

#include "rpc/GatewayRPC.h"
#include "util/SecureXmlParser.h"
#include "util/Loggable.h"

namespace BeeeOn {

class RPCConnector;

class LegacyGatewayRPC: public GatewayRPC, public Loggable {
public:
	LegacyGatewayRPC();

	void setRPCConnector(RPCConnector *connector)
	{
		m_connector = connector;
	}

	void sendListen(const Gateway &gateway) override;
	void unpairDevice(const Gateway &gateway, const Device &device) override;
	void pingGateway(const Gateway &gateway) override;

private:
	void parseResponse(const std::string &response);

	Poco::LogStream m_logStream;
	Poco::Mutex m_lock;
	RPCConnector *m_connector;
	SecureXmlParser m_xmlParser;
};

}

#endif
