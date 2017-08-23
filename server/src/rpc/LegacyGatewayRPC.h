#ifndef BEEEON_LEGACY_GATEWAY_RPC_H
#define BEEEON_LEGACY_GATEWAY_RPC_H

#include <Poco/LogStream.h>
#include <Poco/Mutex.h>
#include <Poco/Timespan.h>

#include "rpc/GatewayRPC.h"
#include "util/AsyncExecutor.h"
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

	void setAsyncExecutor(Poco::SharedPtr<AsyncExecutor> executor)
	{
		m_executor = executor;
	}

	void sendListen(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Poco::Timespan &duration) override;
	void unpairDevice(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Device &device) override;
	void pingGateway(
			const ResultCall &resultCall,
			const Gateway &gateway) override;
	void updateActor(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Device &device,
			const ModuleInfo &module,
			double value,
			const Poco::Timespan &timeout) override;

	void setResponseTimeout(int ms);

private:
	std::string receiveResponse();
	GatewayRPCResult::Ptr parseResponse(const std::string &response);

	Poco::Timespan m_timeout;
	Poco::LogStream m_logStream;
	Poco::Mutex m_lock;
	Poco::SharedPtr<AsyncExecutor> m_executor;
	RPCConnector *m_connector;
	SecureXmlParser m_xmlParser;
};

}

#endif
