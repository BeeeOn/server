#ifndef BEEEON_GATEWAY_REST_HANDLER_H
#define BEEEON_GATEWAY_REST_HANDLER_H

#include "rest/JSONRestHandler.h"

namespace BeeeOn {

class GatewayService;

namespace RestUI {

class GatewayRestHandler : public JSONRestHandler {
public:
	GatewayRestHandler();

	void setGatewayService(GatewayService *service);

	void list(RestFlow &flow);
	void detail(RestFlow &flow);
	void update(RestFlow &flow);
	void assign(RestFlow &flow);
	void unassign(RestFlow &flow);

private:
	GatewayService *m_service;
};

}
}

#endif
