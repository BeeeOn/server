#ifndef BEEEON_UI_GATEWAY_HANDLER_H
#define BEEEON_UI_GATEWAY_HANDLER_H

#include "ui/Handler.h"

namespace Poco {
	class Logger;
}

namespace BeeeOn {
namespace UI {

class GatewayHandler : public Handler {
public:
	/**
	  * Assign a gateway to a place both specified in URI.
	  * If new values for gateway are included they will be updated as well
	  * (i.e. name,location, altitude).
	  */
	static void handlePost(UIRouteContext &context);

	/**
	  * Update gateway properties.
	  */
	static void handlePut(UIRouteContext &context);

	/**
	  * Fetch specified gateway.
	  */
	static void handleGet(UIRouteContext &context);

	/**
	  * Unassign gateway from place. This does not delete the gateway from DB.
	  */
	static void handleDelete(UIRouteContext &context);

protected:
	static void handlePost(Poco::Logger &logger,
			UIRequest &request,
			UIResponse &response,
			GatewayService &gatewayService,
			const UIRoute::Params &params);
	static void handlePut(Poco::Logger &logger,
			UIRequest &request,
			UIResponse &response,
			GatewayService &gatewayService,
			const UIRoute::Params &params);
	static void handleGet(Poco::Logger &logger,
			UIRequest &request,
			UIResponse &response,
			GatewayService &gatewayService,
			const UIRoute::Params &params);
	static void handleDelete(Poco::Logger &logger,
			UIRequest &request,
			UIResponse &response,
			GatewayService &gatewayService,
			const UIRoute::Params &params);
};

}
}

#endif
