#ifndef BEEEON_TIME_ZONE_REST_HANDLER_H
#define BEEEON_TIME_ZONE_REST_HANDLER_H

#include "l10n/TimeZoneProvider.h"
#include "rest/JSONRestHandler.h"

namespace BeeeOn {
namespace RestUI {

class TimeZoneRestHandler : public JSONRestHandler {
public:
	TimeZoneRestHandler();

	void setTimeZoneProvider(TimeZoneProvider::Ptr provider);

	void list(RestFlow &flow);

private:
	TimeZoneProvider::Ptr m_provider;
};

}
}

#endif


