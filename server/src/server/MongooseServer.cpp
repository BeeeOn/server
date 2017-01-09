#include "server/MongooseServer.h"

using namespace BeeeOn;

/**
 * Low-level mongoose request handler. It just redirects requests
 * back to the C++ like style. The wrapper is necessary to bridge
 * with the C-implementation.
 *
 * The MongooseServer handles the actual request. We do some unpacking
 * for simplification.
 */
static void mongoose_handler(struct mg_connection *conn, int ev, void *ev_data)
{
	if (ev != MG_EV_HTTP_REQUEST)
		return;

	assert(conn->mgr->user_data != NULL);
	MongooseServer *server = (MongooseServer *) conn->mgr->user_data;
	struct http_message *msg = (struct http_message *) ev_data;

	try {
		server->handle(conn, msg);
	}
	catch(...) {
		LOGGER_FUNC(__func__).critical("unknown exception, something is really broken");
	}
}

MongooseServer::MongooseServer(unsigned int port,
		Poco::SharedPtr<RequestHandlerFactory> factory,
		unsigned int milis):
	m_activity(this, &MongooseServer::loop),
	m_mg(port, mongoose_handler, this),
	m_factory(factory),
	m_timeout(milis)
{
}
