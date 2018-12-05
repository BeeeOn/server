#pragma once

#include <string>

#include "fcm/FCMReceiverConfigurable.h"
#include "rest/JSONRestHandler.h"
#include "service/FCMTokenService.h"

namespace BeeeOn {
namespace RestUI {

class FCMTokenRestHandler : public JSONRestHandler, public FCMReceiverConfigurable {
public:
	FCMTokenRestHandler();

	void setFCMTokenService(FCMTokenService::Ptr service);

	/**
	 * @brief List all available notification services the client
	 * can register with.
	 *
	 * - usual responses: 200
	 * - example output (data):
	 * <pre>
	 * [
	 *   {
	 *     "name": "fcm",
	 *     "id": "super-public-fcm-id"
	 *   }
	 * ]
	 * </pre>
	 */
	void listServices(RestFlow &flow);

	/**
	 * @brief Register the given token of a notification service
	 * that would be used for client notifications.
	 *
	 * - usual responses: 204
	 * - example input:
	 * <pre>
	 * {
	 *   "id": "super-token-from-fcm"
	 * }
	 * </pre>
	 */
	void registerToken(RestFlow &flow);

	/**
	 * @brief Unregister the given token to stop generating
	 * notifications for the associated client.
	 *
	 * - usual responses: 204
	 * - example input:
	 * <pre>
	 * {
	 *   "id": "super-token-from-fcm"
	 * }
	 * </pre>
	 */
	void unregisterToken(RestFlow &flow);

protected:
	FCMToken extractToken(RestFlow &flow) const;

private:
	FCMTokenService::Ptr m_service;
};

}
}
