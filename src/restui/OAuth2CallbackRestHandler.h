#pragma once

#include "rest/RestHandler.h"

namespace BeeeOn {
namespace RestUI {

class OAuth2CallbackRestHandler : public RestHandler {
public:
	OAuth2CallbackRestHandler();

	/**
	 * @brief It generates a static output "oauth2callback" and its
	 * purpose is just to return HTTP 200.
	 */
	void callback(RestFlow &flow);
};

}
}
