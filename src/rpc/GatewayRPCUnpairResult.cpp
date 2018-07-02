#include "rpc/GatewayRPCUnpairResult.h"

using namespace std;
using namespace BeeeOn;

GatewayRPCUnpairResult::GatewayRPCUnpairResult()
{
}

void GatewayRPCUnpairResult::setUnpaired(const set<DeviceID> &devices)
{
	m_unpaired = devices;
}

const set<DeviceID> &GatewayRPCUnpairResult::unpaired() const
{
	return m_unpaired;
}
