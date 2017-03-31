#ifndef BEEEON_BUILTIN_REST_HANDLER_H
#define BEEEON_BUILTIN_REST_HANDLER_H

#include "di/Injectable.h"
#include "rest/JSONRestHandler.h"

namespace BeeeOn {

class BuiltinRestHandler : public JSONRestHandler {
public:
	BuiltinRestHandler();

	void unauthorized(RestFlow &flow);
	void noRoute(RestFlow &flow);
};

}

#endif
