#pragma once

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
