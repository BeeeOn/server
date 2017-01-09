#ifndef BEEEON_MONGOOSE_SERVER_H
#define BEEEON_MONGOOSE_SERVER_H

#include <cstring>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Activity.h>
#include <Poco/SharedPtr.h>

#include "server/Server.h"
#include "server/RestRequestHandler.h"
#include "util/Loggable.h"
#include "Debug.h"
#include "mongoose/Mongoose.h"

namespace BeeeOn {

/**
 * Mongoose server representation. It just wraps the Mongoose into
 * the Server API to plug into the application
 */
class MongooseServer : public Server, public Loggable {
public:
	typedef TRestRequestHandler<MongooseRequest, MongooseResponse>
		RequestHandler;
	typedef TRestRequestHandlerFactory<MongooseRequest, MongooseResponse>
		RequestHandlerFactory;

	MongooseServer(unsigned int port,
			Poco::SharedPtr<RequestHandlerFactory> factory,
			unsigned int milis = 1000);

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
			logger().log(e, __FILE__, __LINE__);
		}
	}

	void handle(struct mg_connection *conn, struct http_message *msg)
	{
		_TRACE_METHOD(logger());

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
			logger().warning("factory returned NULL",
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
	Poco::Activity<MongooseServer> m_activity;
	Mongoose m_mg;
	Poco::SharedPtr<RequestHandlerFactory> m_factory;
	unsigned int m_timeout;
};

}

#endif
