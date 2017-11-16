#ifndef BEEEON_GWS_GATEWAY_SERVICE_H
#define BEEEON_GWS_GATEWAY_SERVICE_H

#include <Poco/SharedPtr.h>

#include "model/Gateway.h"
#include "model/GatewayStatus.h"

namespace BeeeOn {

class GWSGatewayService {
public:
	typedef Poco::SharedPtr<GWSGatewayService> Ptr;

	virtual ~GWSGatewayService();

	/**
	 * Register the given gateway with the given gateway status.
	 *
	 * It is intended to use after gateway connecting to the server.
	 * In case of the first registration, a new gateway is created
	 * and its status is stored, otherwise only status is stored.
	 *
	 * @return false if gateway or status insertion fails.
	 */
	virtual bool registerGateway(GatewayStatus &status, Gateway &gateway) = 0;
};

}

#endif
