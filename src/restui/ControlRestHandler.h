#pragma once

#include <Poco/Timespan.h>

#include "l10n/Translator.h"
#include "rest/JSONRestHandler.h"
#include "service/ControlService.h"

namespace BeeeOn {

class Control;

namespace RestUI {

class ControlRestHandler : public JSONRestHandler {
public:
	ControlRestHandler();

	void setControlService(ControlService::Ptr service);
	void setTranslatorFactory(TranslatorFactory::Ptr factory);
	void setRequestTimeout(const Poco::Timespan &timeout);

	/**
	 * @brief List all available controllable modules of the
	 * selected device.
	 *
	 * - usual responses: 200
	 * - example output (data):
	 * <pre>
	 * [
	 *   {
	 *     "id": "3",
	 *     "display_name": "On/Off",
	 *     "type_id": "on_off",
	 *     "order": "3",
	 *     "state": {
	 *       "requested_value": 0,
	 *       "requested_at": 1543947401,
	 *       "accepted_at": 1543947402,
	 *       "finished_at": 1543947411,
	 *       "result": "success"
	 *     }
	 *     "current": {
	 *       "value": 0,
	 *       "at": 1543947610
	 *     }
	 *   },
	 *   {
	 *     "id": "4",
	 *     "display_name": "Operation Mode",
	 *     "type_id": "enum",
	 *     "subtype_id": "OPERATION_MODE",
	 *     "order": "4",
	 *     "state": {
	 *       "requested_value": 2,
	 *       "requested_at": 1543947517,
	 *       "result": "waiting"
	 *     }
	 *     "current": {
	 *       "value": 1,
	 *       "at": 1543947364
	 *     }
	 *   }
	 * ]
	 * </pre>
	 *
	 * @see BeeeOn::RestUI::ControlRestHandler::detail(BeeeOn::RestFlow &)
	 */
	void list(RestFlow &flow);

	/**
	 * @brief Show details of a controllable module for the selected
	 * device by its device-unique ID.
	 *
	 * Special properties:
	 * * group - allows to semantically group the module in UI
	 * * order - recommendation how to order that module in UI
	 * * current - if missing, nothing has been received from that module yet
	 * * state - if missing, there was no request to change that module yet
	 *
	 * There can be only one request for a single controllable module at
	 * a time. The request has the property "result" explaining its status
	 * (possible values: "waiting", "success", "failure"). If the request
	 * has been already accepted by the appropriate gateway, the property
	 * "accepted_at" is non-null and holding the time of the acceptation.
	 * If the request is finished (successfully or insuccessfully), the
	 * property "finished_at" is non-null and holding the time of the
	 * finalization.
	 *
	 * - usual responses: 200, 404
	 * - example output (data):
	 * <pre>
	 * {
	 *   "id": "0",
	 *   "display_name": "Target temperature",
	 *   "type_id": "temperature",
	 *   "group": "Thermostat",
	 *   "order": "0",
	 *   "current": {
	 *     "value": 15,
	 *     "at": 1543947821
	 *   },
	 *   "state": {
	 *     "requested_value": 15,
	 *     "requested_at": 1543947820,
	 *     "accepted_at": 1543947820,
	 *     "finished_at": 1543947821,
	 *     "result": "success"
	 *   }
	 * }
	 * </pre>
	 */
	void detail(RestFlow &flow);

	/**
	 * @brief Show current data and state of the selected controllable module.
	 *
	 * - usual responses: 200, 404
	 * - example output (data):
	 * <pre>
	 * {
	 *   "state": {
	 *     "requested_value": 20,
	 *     "requested_at": 1543948549,
	 *     "finished_at": 1543948571,
	 *     "result": "failure"
	 *   }
	 *   "current": {
	 *     "value": 15,
	 *     "at": 1543947821
	 *   }
	 * }
	 * </pre>
	 */
	void current(RestFlow &flow);

	/**
	 * @brief Initiate a request to change state of the selected controllable
	 * module. Only one request can be active at a time. If there is an already
	 * pending request, it is returned.
	 *
	 * It is possible to cancel the previous request and start the new one by
	 * passing parameter "force" in the URL query part.
	 *
	 * - usual responses: 200, 400, 404
	 * - example input:
	 * <pre>
	 * {
	 *   "value": 1,
	 *   "timeout": 30
	 * }
	 * </pre>
	 * - example output (data):
	 * <pre>
	 * {
	 *   "state": {
	 *     "requested_value": 1,
	 *     "requested_at": 1543948549,
	 *     "result": "waiting"
	 *   }
	 *   "current": {
	 *     "value": 0,
	 *     "at": 1543947821
	 *   }
	 * }
	 * </pre>
	 */
	void requestChange(RestFlow &flow);

protected:
	void prepareDetail(RestFlow &flow, Control &control);

private:
	ControlService::Ptr m_controlService;
	TranslatorFactory::Ptr m_translatorFactory;
	Poco::Timespan m_requestTimeout;
};

}
}
