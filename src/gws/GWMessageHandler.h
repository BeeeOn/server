#pragma once

#include <Poco/SharedPtr.h>

#include "gwmessage/GWMessage.h"
#include "model/GatewayID.h"

namespace BeeeOn {

/**
 * @brief GWMessageHandler is responsible for handling incoming GWMessage.
 */
class GWMessageHandler {
public:
	typedef Poco::SharedPtr<GWMessageHandler> Ptr;

	virtual ~GWMessageHandler();

	/**
	 * @brief Handles GWMessage from the given gateway.
	 *
	 * Throw exception in case of invalid message or some other error, that
	 * can not be handled in the GWMessageHandler.
	 */
	virtual void handle(const GWMessage::Ptr message, const GatewayID &gatewayID) = 0;
};

}
