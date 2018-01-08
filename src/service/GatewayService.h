#ifndef BEEEON_GATEWAY_SERVICE_H
#define BEEEON_GATEWAY_SERVICE_H

#include <vector>

#include <Poco/SharedPtr.h>

#include "model/Device.h"
#include "model/User.h"
#include "model/VerifiedIdentity.h"
#include "model/Gateway.h"
#include "model/GatewayScan.h"
#include "model/LegacyGateway.h"
#include "service/Single.h"
#include "service/Relation.h"

namespace BeeeOn {

class GatewayService {
public:
	typedef Poco::SharedPtr<GatewayService> Ptr;

	virtual ~GatewayService();

	/**
	 * Register the given gateway to be owned by the given identity.
	 *
	 * @throw NotFoundException when the gateway does not exist
	 * @throw InvalidArgumentException for invalid identity
	 *
	 * @return false when assignment fails (update operation fails)
	 */
	virtual bool registerGateway(SingleWithData<Gateway> &input,
			const VerifiedIdentity &verifiedIdentity) = 0;
	virtual bool fetch(Single<Gateway> &input) = 0;
	virtual bool fetch(Single<LegacyGateway> &input) = 0;
	virtual void fetchAccessible(Relation<std::vector<Gateway>, User> &input) = 0;
	virtual void fetchAccessible(Relation<std::vector<LegacyGateway>, User> &input) = 0;
	virtual bool update(SingleWithData<Gateway> &input) = 0;
	virtual bool unregister(Single<Gateway> &input) = 0;

	virtual GatewayScan scanDevices(Single<Gateway> &input, const Poco::Timespan &duration) = 0;
	virtual GatewayScan scanStatus(Single<Gateway> &input) = 0;
};

}

#endif
