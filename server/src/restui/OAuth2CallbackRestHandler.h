#ifndef BEEEON_OATUH2_CALLBACK_REST_HANDLER_H
#define BEEEON_OATUH2_CALLBACK_REST_HANDLER_H

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

#endif


