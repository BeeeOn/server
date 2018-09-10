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

	/**
	 * @brief List all available gateways of the current user.
	 *
	 * - usual responses: 200
	 * - example output (data):
	 * <pre>
	 * [
	 *   {
	 *     "id": "1386756787219335",
	 *     "name": "My gateway #1",
	 *     "altitude": 51.5,
	 *     "latitude": -80.673333,
	 *     "longitude": -106.433917,
	 *     "timezone": "en_US",
	 *     "state": {
	 *       "lastModified": 1536588606,
	 *       "ip": "192.168.0.10",
	 *       "version": "v2018.07.1"
	 *     },
	 *     "last_activity": null
	 *   },
	 *   {
	 *     "id": "1502654506100304",
	 *     "name": "My gateway #2",
	 *     "altitude": 1230.12,
	 *     "latitude": 59.297259,
	 *     "longitude": 47.635309,
	 *     "timezone": "en_US",
	 *     "state": {
	 *       "lastModified": 1536588606,
	 *       "ip": "192.168.0.10",
	 *       "version": "v2018.07.2"
	 *     },
	 *     "last_activity": 1536592114,
	 *   }
	 * ]
	 * </pre>
	 */
	void list(RestFlow &flow);

	/**
	 * @brief Show details of a gateway by the given unique gateway ID.
	 *
	 * - usual responses: 200, 404
	 * - example output (data):
	 * <pre>
	 * {
	 *   "id": "1753615168990499",
	 *   "name": "My gateway #1",
	 *   "altitude": 50.0,
	 *   "latitude": 80.0,
	 *   "longitude": 106.0,
	 *   "timezone": "en_US",
	 *   "state": {
	 *     "lastModified": 1536588606,
	 *     "ip": "192.168.0.10",
	 *     "version": "v2018.07.1"
	 *   },
	 *   "last_activity": 1536592114,
	 * }
	 * </pre>
	 */
	void detail(RestFlow &flow);

	/**
	 * @brief Update settings of a gateway specified by ID.
	 * It is possible to update fields: <em>name</em>, <em>timezone_id</em>,
	 * <em>altitude</em>, <em>latitude</em>, <em>longitude</em>.
	 *
	 * - usual responses: 200, 404, 400
	 * - example input:
	 * <pre>
	 * {
	 *    "name": "My second cool gw",
	 *    "timezone_id": "en_GB"
	 * }
	 * </pre>
	 * - example output (data):
	 * <pre>
	 * {
	 *   "id": "1753615168990499",
	 *   "name": "My second cool gw",
	 *   "altitude": 50.0,
	 *   "latitude": 80.0,
	 *   "longitude": 106.0,
	 *   "timezone": "en_GB",
	 *   "state": {
	 *     "lastModified": 1536588606,
	 *     "ip": "192.168.0.10",
	 *     "version": "v2018.07.1"
	 *   }
	 *   "last_activity": 1536592114,
	 * }
	 * </pre>
	 */
	void update(RestFlow &flow);

	/**
	 * @brief Register gateway by ID with the current user (from session).
	 * The user becomes an administrator of the gateway. If the gateway is
	 * already registered, this operation would be unsuccessful.
	 *
	 * It is mandatory to set fields: <em>name</em> and <em>timezone_id</em>.
	 * Optionally, it is possible to specify fields: <em>altitude</em>,
	 * <em>latitude</em> and <em>longitude</em>.
	 *
	 * - Location: {GATEWAY_DETAIL_URI}
	 * - usual responses: 201, 404
	 * - example input:
	 * <pre>
	 * {
	 *    "name": "My cool gw",
	 *    "timezone_id": "pt_PT",
	 *    "altitude": "600"
	 * }
	 * </pre>
	 */
	void assign(RestFlow &flow);

	/**
	 * @brief Unregister gateway by ID from the current user (from session).
	 * If the user is the last administrator of that gateway, there must be
	 * no other users assign for this operation to succeed.
	 *
	 * - Location: {GATEWAY_LIST}
	 * - usual responses: 204, 404
	 */
	void unassign(RestFlow &flow);

private:
	GatewayService *m_service;
	TimeZoneProvider::Ptr m_timeZoneProvider;
};

}
}
