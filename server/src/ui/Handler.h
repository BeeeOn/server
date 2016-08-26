#ifndef BEEEON_UI_HANDLER_H
#define BEEEON_UI_HANDLER_H

#include "server/Route.h"
#include "ui/UIServerModule.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * Handler is a base class for all handlers in UI server.
 *
 * All methods are static in favor of not keeping a context to ensure
 * thread-safety of all handlers.
 */
class Handler {
public:
	static void handleGet(UIRouteContext &context)
	{
		Handler::handleNoRoute(context);
	}

	static void handlePost(UIRouteContext &context)
	{
		Handler::handleNoRoute(context);
	}

	static void handlePut(UIRouteContext &context)
	{
		Handler::handleNoRoute(context);
	}

	static void handleDelete(UIRouteContext &context)
	{
		Handler::handleNoRoute(context);
	}

	static void handleOptions(UIRouteContext &context)
	{
		Handler::handleNoRoute(context);
	}

protected:
	/**
	 * Default handler for a route which has not been implemented yet
	 */
	static void handleNoRoute(UIRouteContext &context)
	{
		TRACE_FUNC();

		context.response().setStatusAndReason(UIResponse::HTTP_NOT_IMPLEMENTED);
	}
};
}
#endif /*BEEEON_UI_HANDLER_H*/
