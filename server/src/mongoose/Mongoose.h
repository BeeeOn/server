#ifndef BEEEON_MONGOOSE_H
#define BEEEON_MONGOOSE_H

#include <cerrno>
#include <cstring>

#include <Poco/Logger.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Exception.h>
#include <mongoose.h>

#include "mongoose/MongooseRequest.h"
#include "mongoose/MongooseResponse.h"
#include "util/Loggable.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * Wrapper around the C-implementation of mongoose.
 */
class Mongoose : public Loggable {
public:
	/**
	 * C-handler for mongoose to be used.
	 * The conn->mgr->user_data holds the user context
	 * given in the Mongoose constructor.
	 */
	typedef void (*Handler)(struct mg_connection *conn,
			int ev, void *ev_data);

	/**
	 * Create mongoose for the given port, handler and pass
	 * user data to be available as a context of the mg_mgr.
	 */
	Mongoose(unsigned int port, Handler handler, void *user):
		m_address("localhost", port),
		m_handler(handler)
	{
		m_mgr = new mg_mgr;
		mg_mgr_init(m_mgr, user);
	}

	~Mongoose()
	{
		mg_mgr_free(m_mgr);
		delete m_mgr;
	}

	void bind()
	{
		_TRACE_METHOD(logger());

		const std::string address = m_address.toString();
		m_conn = mg_bind(m_mgr, address.c_str(), m_handler);
		if (m_conn == NULL) {
			throw Poco::Exception(errno?
				std::strerror(errno) :
				"failed to bind mongoose");
		}

		mg_set_protocol_http_websocket(m_conn);
	}

	void poll(unsigned int milis)
	{
		mg_mgr_poll(m_mgr, milis);
	}

private:
	const Poco::Net::SocketAddress m_address;
	struct mg_mgr *m_mgr;
	struct mg_connection *m_conn;
	Handler m_handler;
};

}

#endif
