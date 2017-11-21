#pragma once

#include <string>

#include <Poco/SharedPtr.h>

#include "fcm/FCMMessage.h"
#include "fcm/FCMResponse.h"

namespace BeeeOn {

/**
 * @brief Application interface that allows sending FCMMessage
 * instances to the Firebase Cloud Messaging service.
 */
class FCMClient {
public:
	typedef Poco::SharedPtr<FCMClient> Ptr;

	virtual ~FCMClient();

	/**
	 * @param message - the message to be sent will be serialized according
	 * to the implementation
	 * @param serverKey - server key for authentication with the FCM service
	 * @param dryRun - if true, the message is sent in dry mode (if supported)
	 */
	virtual FCMResponse sendMessage(
			const FCMMessage &message,
			const std::string &serverKey,
			bool dryRun = false) = 0;
};

}
