#pragma once

#include "l10n/TimeZoneProvider.h"
#include "rest/JSONRestHandler.h"

namespace BeeeOn {

class GatewayService;

namespace RestUI {

class GatewayRestHandler : public JSONRestHandler {
public:
	GatewayRestHandler();

	void setGatewayService(GatewayService *service);
	void setTimeZoneProvider(TimeZoneProvider::Ptr provider);

	void list(RestFlow &flow);
	void detail(RestFlow &flow);
	void update(RestFlow &flow);
	void assign(RestFlow &flow);
	void unassign(RestFlow &flow);

private:
	GatewayService *m_service;
	TimeZoneProvider::Ptr m_timeZoneProvider;
};

}
}
