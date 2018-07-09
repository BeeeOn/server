#pragma once

#include <set>

#include "model/DeviceID.h"
#include "rpc/GatewayRPCResult.h"

namespace BeeeOn {

/**
 * @brief A specialization of the GatewayRPCResult that allows to pass
 * set of device IDs representing devices that were unpaired by the
 * gateway.
 */
class GatewayRPCUnpairResult : public GatewayRPCResult {
public:
	typedef Poco::AutoPtr<GatewayRPCUnpairResult> Ptr;

	GatewayRPCUnpairResult();

	void setUnpaired(const std::set<DeviceID> &devices);

	/**
	 * @returns set of IDs of devices that have been unpaired
	 */
	const std::set<DeviceID> &unpaired() const;

private:
	std::set<DeviceID> m_unpaired;
};

}
