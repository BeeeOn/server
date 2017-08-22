#ifndef BEEEON_GATEWAY_REST_HANDLER_H
#define BEEEON_GATEWAY_REST_HANDLER_H

#include "rest/JSONRestHandler.h"
#include "service/WorkService.h"

namespace BeeeOn {
namespace RestUI {

class WorkRestHandler : public JSONRestHandler {
public:
	WorkRestHandler();

	void setWorkService(WorkService::Ptr service);

	void detail(RestFlow &flow);
	void update(RestFlow &flow);
	void remove(RestFlow &flow);

private:
	WorkService::Ptr m_service;
};

}
}

#endif
