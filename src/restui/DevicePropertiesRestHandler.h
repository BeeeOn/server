#pragma once

#include <Poco/SharedPtr.h>

#include "rest/JSONRestHandler.h"
#include "service/DeviceService.h"
#include "util/CryptoConfig.h"

namespace BeeeOn {

namespace RestUI {

class DevicePropertiesRestHandler : public JSONRestHandler {
public:
	DevicePropertiesRestHandler();

	void setDeviceService(DeviceService::Ptr service);
	void setCryptoConfig(Poco::SharedPtr<CryptoConfig> config);

	/**
	 * @brief List all available property keys that can be used
	 * for device properties.
	 *
	 * - usual responses: 200
	 * - example output (data):
	 * <pre>
	 * [
	 *   {
	 *     "display_name": "IP address",
	 *     "key": "ip_address",
	 *   },
	 *   {
	 *     "display_name": "Password",
	 *     "key": "password",
	 *     "write-only": true
	 *   },
	 *   {
	 *     "display_name": "Firmware",
	 *     "key": "firmware"
	 *     "read-only": frue
	 *   }
	 * ]
	 * </pre>
	 */
	void keys(RestFlow &flow);

	/**
	 * @brief List all properties associated with the requested device.
	 * Each property can be read-only, write-only or both read-writable.
	 * Write-only properties would have value null.
	 *
	 * - usual responses: 200
	 * - example output (data):
	 * <pre>
	 * [
	 *   {
	 *     "display_name": "IP address",
	 *     "key": "ip_address",
	 *     "value": "192.168.0.1"
	 *   },
	 *   {
	 *     "display_name": "Password",
	 *     "key": "password",
	 *     "value": null,
	 *     "write-only": true
	 *   },
	 *   {
	 *     "display_name": "Firmware",
	 *     "key": "firmware",
	 *     "value": "v128",
	 *     "read-only": true
	 *   }
	 * ]
	 * </pre>
	 */
	void list(RestFlow &flow);

	/**
	 * @brief Show the associated property of the selected device.
	 * Write-only properties would have value null.
	 *
	 * - usual responses: 200, 400, 404
	 * - example output (data):
	 * <pre>
	 * {
	 *   "display_name": "IP address",
	 *   "key": "ip_address",
	 *   "value": "10.0.0.1"
	 * }
	 * </pre>
	 */
	void detail(RestFlow &flow);

	/**
	 * @brief Create a new property and associate it with the selected device.
	 * Properties known-to be read-only would not be created and the response
	 * would have HTTP code 405. Creating and already existing property would
	 * generate HTTP code 409.
	 *
	 * - usual responses: 200, 400, 404, 405, 409
	 * - example input (data):
	 * <pre>
	 * {
	 *   "value": "10.0.0.1"
	 * }
	 * </pre>
	 *
	 * - example output (data):
	 * <pre>
	 * {
	 *   "display_name": "IP address",
	 *   "key": "ip_address",
	 *   "value": "10.0.0.1"
	 * }
	 * </pre>
	 */
	void create(RestFlow &flow);

	/**
	 * @brief Update editable property associated with the selected device.
	 * Read-only properties cannot be updated and would generate HTTP code 405.
	 *
	 * - usual responses: 200, 400, 404, 405
	 * - example input:
	 * <pre>
	 * {
	 *   "value": "Ohchoo9Ali6aiCh4azee1Oxa"
	 * }
	 * </pre>
	 *
	 * - example output (data):
	 * <pre>
	 * {
	 *   "display_name": "Password",
	 *   "key": "password",
	 *   "value: null,
	 *   "write-only": true
	 * }
	 * </pre>
	 */
	void update(RestFlow &flow);

	/**
	 * @brief Remove the selected property from the associated device.
	 * Read-only properties cannot be removed and would generate HTTP
	 * code 405.
	 *
	 * - usual responses: 204, 404, 405
	 * - example output (data):
	 * <pre>
	* [
	 *   {
	 *     "display_name": "IP address",
	 *     "key": "ip_address",
	 *     "value": "192.168.0.1",
	 *   },
	 *   {
	 *     "display_name": "Firmware",
	 *     "key": "firmware",
	 *     "value": "v128",
	 *     "read-only": true
	 *   }
	 * ]
	 * </pre>
	 */
	void remove(RestFlow &flow);

private:
	DeviceService::Ptr m_service;
	Poco::SharedPtr<CryptoConfig> m_cryptoConfig;
};

}
}

