#ifndef BEEEON_MONGOOSE_SERVER_H
#define BEEEON_MONGOOSE_SERVER_H

#include <cstring>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Activity.h>
#include <Poco/SharedPtr.h>

#include "server/Server.h"
#include "server/RestRequestHandler.h"
#include "Debug.h"
#include "mongoose/Mongoose.h"

namespace BeeeOn {

template <typename UserData = void *>
class TMongooseServer;

/**
 * Low-level mongoose request handler. It just redirects requests
 * back to the C++ like style. The wrapper is necessary to bridge
 * with the C-implementation.
 *
 * The MongooseServer handles the actual request. We do some unpacking
 * for simplification.
 */
template <typename UserData = void *>
void mongoose_handler(struct mg_connection *conn, int ev, void *ev_data)
{
	using MongooseServer = TMongooseServer<UserData>;

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
		stdout_backtrace();
		log_backtrace(LOGGER_FUNC(__func__));
	}
}

/**
 * Mongoose server representation. It just wraps the Mongoose into
 * the Server API to plug into the application
 */
template <typename UserData>
class TMongooseServer : public Server {
public:
	typedef TRestRequestHandler
		<MongooseRequest, MongooseResponse, UserData>
		RequestHandler;
	typedef TRestRequestHandlerFactory
		<MongooseRequest, MongooseResponse, UserData>
		RequestHandlerFactory;

	TMongooseServer(unsigned int port,
			Poco::SharedPtr<RequestHandlerFactory> factory,
			unsigned int milis = 1000):
		m_activity(this, &TMongooseServer<UserData>::loop),
		m_mg(port, mongoose_handler<>, this),
		m_factory(factory),
		m_timeout(milis),
		m_logger(LOGGER_CLASS(this))
	{
	}

	void start()
	{
		m_activity.start();
	}

	void stop()
	{
		stop(1000);
	}

	void stop(long milis)
	{
		m_activity.stop();

		try {
			m_activity.wait(milis);
		} catch(Poco::TimeoutException &e) {
			m_logger.log(e, __FILE__, __LINE__);
		}
	}

	void handle(struct mg_connection *conn, struct http_message *msg)
	{
		_TRACE_METHOD(m_logger);

		bool isHead = !std::strncmp(
				msg->method.p, "HEAD", msg->method.len);
		MongooseResponse response(conn, isHead);

		Poco::Net::SocketAddress address(
				conn->sa.sa, sizeof(conn->sa));
		MongooseRequest request(address, msg, response);

		RequestHandler *handler;
		handler = m_factory->createRequestHandler(request);

		/* There should never be handler == NULL */
		if (handler == NULL) {
			m_logger.warning("factory returned NULL",
					__FILE__, __LINE__);
			response.setStatusAndReason(
				MongooseResponse::HTTP_INTERNAL_SERVER_ERROR);
		}
		else {
			/* proper request handling */
			handler->handleRequest(request, response);
			delete handler;
		}

		if (!response.sent())
			response.send().flush();
	}

private:
	void loop()
	{
		m_mg.bind();
		m_factory->reportRoutes();

		while (!m_activity.isStopped())
			m_mg.poll(m_timeout);
	}

private:
	Poco::Activity<TMongooseServer<UserData>> m_activity;
	Mongoose m_mg;
	Poco::SharedPtr<RequestHandlerFactory> m_factory;
	unsigned int m_timeout;
	Poco::Logger &m_logger;
};

}

#endif
