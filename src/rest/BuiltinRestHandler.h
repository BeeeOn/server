#pragma once

#include "di/Injectable.h"
#include "rest/JSONRestHandler.h"

namespace BeeeOn {

class BuiltinRestHandler : public JSONRestHandler {
public:
	BuiltinRestHandler();

	/**
	 * @brief It always returns HTTP code 401 and generates HTTP header
	 * WWW-Authenticate: Basic realm="BeeeOn Server" (the realm is configurable
	 * via the used RestRouter).
	 * @throws Poco::Net::NotAuthenticatedException - always
	 */
	void unauthorized(RestFlow &flow);

	/**
	 * @brief It always returns HTTP code 404.
	 * @throws Poco::NotFoundException - always
	 */
	void noRoute(RestFlow &flow);
};

}
