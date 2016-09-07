#ifndef BEEEON_UI_LOCATION_HANDLER_H
#define BEEEON_UI_LOCATION_HANDLER_H

#include "ui/Handler.h"

namespace Poco {
	class Logger;
}

namespace BeeeOn {
namespace UI {

class LocationHandler : public Handler {
public:
	static void handlePost(UIRouteContext &context);
	static void handlePut(UIRouteContext &context);
	static void handleGet(UIRouteContext &context);
	static void handleDelete(UIRouteContext &context);

protected:
	static void handlePost(Poco::Logger &logger,
			UIRequest &request,
			UIResponse &response,
			LocationService &service,
			const UIRoute::Params &params);
	static void handlePut(Poco::Logger &logger,
			UIRequest &request,
			UIResponse &response,
			LocationService &service,
			const UIRoute::Params &params);
	static void handleGet(Poco::Logger &logger,
			UIRequest &request,
			UIResponse &response,
			LocationService &service,
			const UIRoute::Params &params);
	static void handleDelete(Poco::Logger &logger,
			UIRequest &request,
			UIResponse &response,
			LocationService &service,
			const UIRoute::Params &params);
};

}
}

#endif
