#include <Poco/Nullable.h>
#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "l10n/TimeZone.h"
#include "rest/RestFlow.h"
#include "restui/Serializing.h"
#include "restui/TimeZoneRestHandler.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, TimeZoneRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_PROPERTY("timeZoneProvider", &TimeZoneRestHandler::setTimeZoneProvider)
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
	registerAction<TimeZoneRestHandler>("detail", &TimeZoneRestHandler::detailByID, {"id"});
	registerAction<TimeZoneRestHandler>("detail_pair", &TimeZoneRestHandler::detailByPair, {"continent", "city"});
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

void TimeZoneRestHandler::detailByID(RestFlow &flow)
{
	const string &id = Sanitize::common(flow.param("id"));

	detail(flow, id);
}

void TimeZoneRestHandler::detailByPair(RestFlow &flow)
{
	const string &continent = Sanitize::common(flow.param("continent"));
	const string &city = Sanitize::common(flow.param("city"));

	detail(flow, continent + "/" + city);
}

void TimeZoneRestHandler::detail(RestFlow &flow, const std::string &id)
{
	Nullable<TimeZone> zone = m_provider->findById(id);
	if (zone.isNull())
		throw NotFoundException("no such zone");

	PrintHandler result(flow.response().stream());

	beginSuccess(result, 200);
	serialize(result, zone, flow.locale());
	endSuccess(result);
}
