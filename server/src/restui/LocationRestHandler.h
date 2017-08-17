#ifndef BEEEON_LOCATION_REST_HANDLER_H
#define BEEEON_LOCATION_REST_HANDLER_H

#include "rest/JSONRestHandler.h"
#include "service/LocationService.h"

namespace BeeeOn {
namespace RestUI {

class LocationRestHandler : public JSONRestHandler {

public:
	LocationRestHandler();

	void setLocationService(LocationService::Ptr service);

	void list(RestFlow &flow);
	void detail(RestFlow &flow);
	void create(RestFlow &flow);
	void update(RestFlow &flow);
	void remove(RestFlow &flow);

private:
	LocationService::Ptr m_locationService;
};

}
}

#endif
