#pragma once

#include <Poco/SharedPtr.h>

#include "rest/JSONRestHandler.h"

namespace BeeeOn {

class DeviceService;
class GatewayService;

namespace RestUI {

class DeviceRestHandler : public JSONRestHandler {
public:
	DeviceRestHandler();

	void setDeviceService(Poco::SharedPtr<DeviceService> service);
	void setGatewayService(Poco::SharedPtr<GatewayService> service);

	/**
	 * @brief List all devices associated with the given gateway.
	 *
	 * - usual responses: 200
	 * - example output (data):
	 * <pre>
	 * [
	 *   {
	 *     "id": "0xa300000000000001",
	 *     "location_id: "c5c8c9f1-e501-42fd-8f4d-31744c8eeacc",
	 *     "name": "My favourite sensor",
	 *     "type": {
	 *       "name": "SuperS-3000X",
	 *       "display_name": "Super Sensor 3000",
	 *       "name": "Super Company"
	 *     },
	 *     "family": "vdev",
	 *     "first_seen": 1538567860,
	 *     "last_seen": 1538569021,
	 *     "active_since": 1538568123,
	 *     "last_changed": 1538568123,
	 *     "state": "active",
	 *     "refresh_time": 30,
	 *     "available": true
	 *   },
	 *   {
	 *     "id": "0xa600001012ad4912",
	 *     "location_id: "00000000-0000-0000-0000-000000000000",
	 *     "name": "My mobile phone",
	 *     "type": {
	 *       "name": "Unknown device",
	 *       "display_name": "Unknown device",
	 *       "name": "Bluetooth Availability"
	 *     },
	 *     "family": "bluetooth",
	 *     "first_seen": 1538565192,
	 *     "last_seen": 1538849021,
	 *     "last_changed": 1538569021,
	 *     "state": "inactive",
	 *     "available": false
	 *   }
	 * ]
	 * </pre>
	 */
	void list(RestFlow &flow);

	/**
	 * @brief Show details of a device by the gateway ID and device ID.
	 * The property "active_since" is available only in case, when the
	 * device status is "active". The property "last_changed" is a more
	 * general and tells when the most recent state was changed. This
	 * allows to determine both when the device became "active" (same
	 * as "active_since") and when the device became "inactive".
	 *
	 * - usual responses: 200, 404
	 * - example output (data):
	 * <pre>
	 * {
	 *   "id": "0xa300000000000001",
	 *   "location_id: "c5c8c9f1-e501-42fd-8f4d-31744c8eeacc",
	 *   "name": "My favourite sensor",
	 *   "type": {
	 *     "name": "SuperS-3000X",
	 *     "display_name": "Super Sensor 3000",
	 *     "name": "Super Company"
	 *   },
	 *   "family": "vdev",
	 *   "first_seen": 1538567860,
	 *   "last_seen": 1538569021,
	 *   "active_since": 1538568123,
	 *   "last_changed": 1538568123,
	 *   "state": "active",
	 *   "refresh_time": 30,
	 *   "available": true
	 * }
	 * </pre>
	 */
	void detail(RestFlow &flow);

	/**
	 * @brief Update editable information of a device. Note, that the
	 * refresh time might not be updatable for certain devices.
	 *
	 * - usual responses: 200, 400, 404
	 * - example input:
	 * <pre>
	 * {
	 *   "name": "My coolest device",
	 *   "location_id": "d087483a-a3ea-4736-ab2d-5f479bb16bfc",
	 *   "refresh_time": 50
	 * }
	 * </pre>
	 *
	 * - example output (data):
	 * <pre>
	 * {
	 *   "id": "0xa300000000000002",
	 *   "location_id: "d087483a-a3ea-4736-ab2d-5f479bb16bfc",
	 *   "name": "My coolest device",
	 *   "type": {
	 *     "name": "SuperS-3000X",
	 *     "display_name": "Super Sensor 3000",
	 *     "name": "Super Company"
	 *   },
	 *   "family": "vdev",
	 *   "first_seen": 1538567860,
	 *   "last_seen": 1538569021,
	 *   "active_since": 1538568123,
	 *   "last_changed": 1538568123,
	 *   "state": "active",
	 *   "refresh_time": 50,
	 *   "available": true
	 * }
	 * </pre>
	 */
	void update(RestFlow &flow);

	/**
	 * @brief Start discovery process on the associated gateway.
	 * The gateway would search for non-paired devices and report
	 * them to the server. Some technologies might require a manual
	 * intervation on the side of the gateway.
	 *
	 * - Location: {DEVICE_DISCOVERY_STATUS_URI}
	 * - usual responses: 202, 400
	 * - example input:
	 * <pre>
	 * {
	 *   "time_limit": 30
	 * }
	 * </pre>
	 *
	 * - example output (data):
	 * <pre>
	 * {
	 *   "duration": 30,
	 *   "started": 1538569108,
	 *   "state": {
	 *     "name": "waiting",
	 *     "display_name": "In progress"
	 *   }
	 * }
	 * </pre>
	 */
	void discover(RestFlow &flow);

	/**
	 * - usual responses: 200, 404
	 * - example output - idle (data):
	 * <pre>
	 * {
	 *   "state": {
	 *     "name": "idle",
	 *     "display_name": "Idle"
	 *   }
	 * }
	 * </pre>
	 * - example output - waiting (data):
	 * <pre>
	 * {
	 *   "duration": 30,
	 *   "started": 1538569108,
	 *   "state": {
	 *     "name": "waiting",
	 *     "display_name": "In progress"
	 *   }
	 * }
	 * </pre>
	 * - other possible states: processing, finished, failed *
	 */
	void discoverStatus(RestFlow &flow);

	/**
	 * @brief Unassociate a device from its gateway. Such device
	 * is not deleted, but it is no longer receiving data and cannot
	 * be remotely controlled in any other way. The semantics of
	 * the unpair operation might vary for different technologies.
	 * Some technologies would disassociate the device entirely,
	 * others would just stop reporting it at the highest level.
	 *
	 * - usual responses: 202, 404
	 * - example output (data):
	 * <pre>
	 * {
	 *   "id": "0xa300000000000002",
	 *   "location_id: "d087483a-a3ea-4736-ab2d-5f479bb16bfc",
	 *   "name": "My coolest device",
	 *   "type": {
	 *     "name": "SuperS-3000X",
	 *     "display_name": "Super Sensor 3000",
	 *     "name": "Super Company"
	 *   },
	 *   "family": "vdev",
	 *   "first_seen": 1538567860,
	 *   "last_seen": 1538569021,
	 *   "last_changed": 1538568123,
	 *   "state": "inactive",
	 *   "refresh_time": 50,
	 *   "available": true
	 * }
	 * </pre>
	 */
	void unpair(RestFlow &flow);

private:
	Poco::SharedPtr<DeviceService> m_deviceService;
	Poco::SharedPtr<GatewayService> m_gatewayService;
};

}
}
