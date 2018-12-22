#pragma once

#include "service/GatewayMessageService.h"
#include "rest/JSONRestHandler.h"

namespace BeeeOn {
namespace RestUI {

class MessageRestHandler : public JSONRestHandler {
public:
	MessageRestHandler();

	void setGatewayMessageService(GatewayMessageService::Ptr service);

	/**
	 * @brief List all messages related to the given gateway.
	 *
	 * - usual responses: 200
	 * - example output (data):
	 * <pre>
	 * [
	 *   {
	 *     "id": "28065460-9fd5-4471-bc5e-4bae866f2743",
	 *     "gateway_id": "1667652370823517",
	 *     "at": 1546265027587,
	 *     "severity": "warn",
	 *     "key": "discovery.failed",
	 *     "context": {"gateway_id": "1667652370823517", "technology": "bluetooth"},
	 *     "display_text": "Discovery failed for 'bluetooth' on gateway 1667652370823517"
	 *   },
	 *   {
	 *     "id": "df616fe5-5fa6-43fc-800c-ffb9bc23d72a",
	 *     "gateway_id": "1667652370823517",
	 *     "at": 1546269212092,
	 *     "severity": "info",
	 *     "key": "control.changed",
	 *     "context": {
	 *        "gateway_id": "1667652370823517",
	 *        "device_name": "Lights",
	 *        "device_id": "0xa712345600010203",
	 *        "module_id": 2,
	 *        "module_type": "on_off"
	 *     },
	 *     "display_text": "Control 'on_off' was successfully changed for device 'Lights'"
	 *   }
	 * ]
	 * </pre>
	 *
	 * @see BeeeOn::RestUI::MessageRestHandler::detail
	 */
	void list(RestFlow &flow);

	/**
	 * @brief List all messages related to the given gateway.
	 * The result contains the message description consisting of:
	 * - at - time of the message creation in milliseconds
	 * - severity - "info", "warn" or "error"
	 * - key - string that identifies the message
	 * - context - optional JSON object describing the context of the message
	 * - display_text - translated message composed from the key and context
	 *
	 * The <code>display_text</code> entry would be missing if such message
	 * is not defined as a resource on the server. The client can still try
	 * to translate the message on its own based on the _key_ and _context_.
	 *
	 * - usual responses: 200, 404
	 * - example output (data):
	 * <pre>
	 * {
	 *   "id": "28065460-9fd5-4471-bc5e-4bae866f2743",
	 *   "gateway_id": "1667652370823517",
	 *   "at": 1546265027587,
	 *   "severity": "warn",
	 *   "key": "discovery.failed",
	 *   "context": {"gateway_id": "1667652370823517", "technology": "bluetooth"},
	 *   "display_text": "Discovery failed for 'bluetooth' on gateway 1667652370823517"
	 * }
	 * </pre>
	 */
	void detail(RestFlow &flow);

	/**
	 * @brief Remove the selected message of a gateway.
	 *
	 * - Location: {MESSAGES_LIST}
	 * - usual responses: 204, 404
	 */
	void remove(RestFlow &flow);

private:
	GatewayMessageService::Ptr m_service;
};

}
}
