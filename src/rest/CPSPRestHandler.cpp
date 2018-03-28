#include "di/Injectable.h"
#include "rest/CPSPRestHandler.h"
#include "rest/RestFlow.h"
#include "util/ClassInfo.h"

BEEEON_OBJECT_BEGIN(BeeeOn, CPSPRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_PROPERTY("handlers", &CPSPRestHandler::registerHandler)
BEEEON_OBJECT_END(BeeeOn, CPSPRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

CPSPRestHandler::CPSPRestHandler():
	RestHandler("cpsp")
{
}

void CPSPRestHandler::registerHandler(SharedPtr<HTTPRequestHandler> handler)
{
	ClassInfo clazz(*handler);

	registerAction(clazz.name(),
		[handler](RestFlow &flow) mutable {
			handler->handleRequest(
				flow.request().impl(),
				flow.response().impl());
		}
	);
}
