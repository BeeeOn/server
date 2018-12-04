#pragma once

#include "l10n/TimeZoneProvider.h"
#include "rest/JSONRestHandler.h"

namespace BeeeOn {
namespace RestUI {

class TimeZoneRestHandler : public JSONRestHandler {
public:
	TimeZoneRestHandler();

	void setTimeZoneProvider(TimeZoneProvider::Ptr provider);

	/**
	 * @brief List available time zones known to the server.
	 *
	 * - usual responses: 200
	 * - example output (data):
	 * <pre>
	 * [
	 *   {
	 *     "id": "Africa/Abidjan",
	 *     "short_name": "GMT",
	 *     "display_name": "Africa/Abidjan (GMT)",
	 *     "utc_offset": 0,
	 *     "dst_offset": 0,
	 *     "dst_in_effect": 0
	 *   },
	 *   {
	 *     "id": "Africa/Accra",
	 *     "short_name": "GMT",
	 *     "display_name": "Africa/Accra (GMT)",
	 *     "utc_offset": 0,
	 *     "dst_offset": 0,
	 *     "dst_in_effect": 0
	 *   },
	 *   {
	 *     "id": "America/Santiago",
	 *     "short_name": "GMT-03:00",
	 *     "display_name": "America/Santiago (GMT-03:00)",
	 *     "utc_offset": -14400,
	 *     "dst_offset": 3600,
	 *     "dst_in_effect": 1
	 *   },
	 *   {
	 *     "id": "Europe/Budapest",
	 *     "short_name": "GMT+01:00",
	 *     "display_name": "Europe/Budapest (GMT+01:00)",
	 *     "utc_offset": 3600,
	 *     "dst_offset": 3600,
	 *     "dst_in_effect": 0
	 *   },
	 *   {
	 *     "id": "Europe/Prague",
	 *     "short_name": "GMT+01:00",
	 *     "display_name": "Europe/Prague (GMT+01:00)",
	 *     "utc_offset": 3600,
	 *     "dst_offset": 3600,
	 *     "dst_in_effect": 0
	 *   }
	 * ]
	 * </pre>
	 */
	void list(RestFlow &flow);

	/**
	 * @brief Show details of a time zone by its ID.
	 *
	 * Each time zone is described by its unique ID and offsets
	 * from the UTC time. In case when a time zone uses the DST
	 * (daylight saving time), the additional properties are to
	 * be considered:
	 *
	 * * dst_offset - offset when DST is in effect
	 * * dst_in_effect - 1 when it is in effect right know, otherwise 0
	 *
	 * The dst_in_effect property changes during the year according
	 * to the current time and date.
	 *
	 * - usual responses: 200, 404
	 * - example output (data):
	 * <pre>
	 * {
	 *   {
	 *     "id": "Europe/Prague",
	 *     "short_name": "GMT+01:00",
	 *     "display_name": "Europe/Prague (GMT+01:00)",
	 *     "utc_offset": 3600,
	 *     "dst_offset": 3600,
	 *     "dst_in_effect": 0
	 *   }
	 * }
	 * </pre>
	 */
	void detailByID(RestFlow &flow);

	/**
	 * @brief Show details of a time zone by the combination
	 * country/city. The time zone ID is often expressed in
	 * the format country/city which does not fit into the
	 * URL path as 1 single item. This endpoint has thus the
	 * same semantics as detailByID().
	 *
	 * @see BeeeOn::RestUI::TimeZoneRestHandler::detailByID(BeeeOn::RestFlow &)
	 */
	void detailByPair(RestFlow &flow);

protected:
	void detail(RestFlow &flow, const std::string &id);

private:
	TimeZoneProvider::Ptr m_provider;
};

}
}
