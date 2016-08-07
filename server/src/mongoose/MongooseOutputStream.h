#ifndef BEEEON_MONGOOSE_OUTPUT_STREAM_H
#define BEEEON_MONGOOSE_OUTPUT_STREAM_H

#include <iostream>
#include <Poco/BufferedStreamBuf.h>
#include <mongoose.h>

namespace BeeeOn {

/**
 * Buffer for implementing buffered streams by utilizing ostream.
 */
class MongooseStreamBuf : public Poco::BufferedStreamBuf {
public:
	MongooseStreamBuf(struct mg_connection *conn,
			std::streamsize size = 512):
		Poco::BufferedStreamBuf(size, std::ios::out),
		m_conn(conn)
	{
	}

	/**
	 * Send buffered data out via mongoose.
	 */
	int writeToDevice(const char* buffer, std::streamsize length)
	{
		mg_send(m_conn, buffer, length);
		return length;
	}

private:
	struct mg_connection *m_conn;
};

/**
 * Base IOS class for Mongoose. It is required for the right
 * initialization and deinitialization ordering.
 */
class MongooseIOS : public virtual std::ios {
public:
	MongooseIOS(struct mg_connection *conn, std::streamsize size = 512):
		m_buf(conn, size)
	{
	}

protected:
	MongooseStreamBuf m_buf;
};

/**
 * MongooseOutputStream wraps the C-calls to mg_send of mongoose.
 * @see class MongooseStreamBuf
 */
class MongooseOutputStream : public virtual MongooseIOS, public std::ostream {
public:
	MongooseOutputStream(struct mg_connection *conn,
			std::streamsize size = 512):
		MongooseIOS(conn, size),
		std::ostream(&m_buf)
	{
	}
};

}

#endif
