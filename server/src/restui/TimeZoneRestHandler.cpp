#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "l10n/TimeZone.h"
#include "rest/RestFlow.h"
#include "restui/Serializing.h"
#include "restui/TimeZoneRestHandler.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, TimeZoneRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_REF("timeZoneProvider", &TimeZoneRestHandler::setTimeZoneProvider)
BEEEON_OBJECT_END(BeeeOn, RestUI, TimeZoneRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

TimeZoneRestHandler::TimeZoneRestHandler():
	JSONRestHandler("time_zones")
{
	registerAction<TimeZoneRestHandler>("list", &TimeZoneRestHandler::list);
}

void TimeZoneRestHandler::setTimeZoneProvider(TimeZoneProvider::Ptr provider)
{
	m_provider = provider;
}

void TimeZoneRestHandler::list(RestFlow &flow)
{
	vector<TimeZone> zones;
	m_provider->listTimeZones(zones);

	PrintHandler result(flow.response().stream());

	beginSuccess(result, 200);
	serialize(result, zones, flow.locale());
	endSuccess(result);
}
