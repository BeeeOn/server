#include <string>

#include <Poco/Clock.h>
#include <Poco/Logger.h>

#include "server/WebSocketRequestHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

WebSocketRequestHandler::WebSocketRequestHandler()
{
}

WebSocketRequestHandler::~WebSocketRequestHandler()
{
}

void WebSocketRequestHandler::run()
{
	const Clock started;

	try {
		WebSocket ws(request(), response());
		handle(ws);
	}
	BEEEON_CATCH_CHAIN(logger())

	if (logger().information()) {
		logger().information(
			"duration: " + to_string(started.elapsed()) + "us",
			__FILE__, __LINE__);
	}
}

WebSocketRequestHandlerFactory::WebSocketRequestHandlerFactory()
{
}

WebSocketRequestHandlerFactory::~WebSocketRequestHandlerFactory()
{
}

HTTPRequestHandler *WebSocketRequestHandlerFactory::createRequestHandler(
	const HTTPServerRequest &request)
{
	return createWSHandler(request);
}
