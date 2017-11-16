#include <Poco/Exception.h>
#include <Poco/Net/NetException.h>

#include "rest/BuiltinRestHandler.h"

BEEEON_OBJECT_BEGIN(BeeeOn, BuiltinRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_END(BeeeOn, BuiltinRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

BuiltinRestHandler::BuiltinRestHandler():
	JSONRestHandler("builtin")
{
	registerAction<BuiltinRestHandler>("unauthorized", &BuiltinRestHandler::unauthorized);
	registerAction<BuiltinRestHandler>("noroute", &BuiltinRestHandler::noRoute);
}

void BuiltinRestHandler::unauthorized(RestFlow &flow)
{
	throw NotAuthenticatedException("missing credentials");
}

void BuiltinRestHandler::noRoute(RestFlow &flow)
{
	throw NotFoundException("no such resource available");
}
