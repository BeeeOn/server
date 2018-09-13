#pragma once

#include "rest/RestHandler.h"

namespace BeeeOn {
namespace RestUI {

class OAuth2CallbackRestHandler : public RestHandler {
public:
	OAuth2CallbackRestHandler();

	void callback(RestFlow &flow);
};

}
}
