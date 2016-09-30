#ifndef BEEEON_MONGOOSE_RESPONSE_H
#define BEEEON_MONGOOSE_RESPONSE_H

#include <map>
#include <iostream>
#include <Poco/NullStream.h>
#include <Poco/Exception.h>

#include "mongoose/MongooseOutputStream.h"

namespace BeeeOn {

/**
 * Represents a mongoose-specific HTTP response. It can be used instead
 * of the Poco HTTPServerResponse (there is an early-binding relation
 * by utilizing C++ templates).
 */
class MongooseResponse {
public:
	typedef std::map<std::string, std::string> HeadersMap;

	static const std::string HTTP_1_0;
	static const std::string HTTP_1_1;

	enum Status {
		HTTP_OK = 200,
		HTTP_MOVED_PERMANENTLY = 301,
		HTTP_FOUND = 302,
		HTTP_NOT_MODIFIED = 304,
		HTTP_BAD_REQUEST = 400,
		HTTP_UNAUTHORIZED = 401,
		HTTP_PAYMENT_REQUIRED = 402,
		HTTP_FORBIDDEN = 403,
		HTTP_NOT_FOUND = 404,
		HTTP_METHOD_NOT_ALLOWED = 405,
		HTTP_NOT_ACCEPTABLE = 406,
		HTTP_REQUESTURITOOLONG = 414,
		HTTP_INTERNAL_SERVER_ERROR = 500,
		HTTP_NOT_IMPLEMENTED = 501,
		HTTP_SERVICE_UNAVAILABLE = 503,
		HTTP_VERSION_NOT_SUPPORTED = 505,
	};

	MongooseResponse(struct mg_connection *conn,
			bool noSendData = false,
			std::streamsize bufsize = 512):
		m_noSendData(noSendData),
		m_output(conn, bufsize),
		m_headersSent(false),
		m_keepAlive(false),
		m_contentLength(-1),
		m_version(HTTP_1_1),
		m_status(HTTP_OK),
		m_reason("OK")
	{
	}

	~MongooseResponse()
	{
		m_output.flush();
	}

	void redirect(const std::string uri, int status);

	/**
	 * Ask for authentication. The scheme is not supported by
	 * Poco::Net::HTTPServerResponse.
	 */
	void requireAuthentication(
			const std::string &realm,
			const std::string &scheme = "Basic");

	void setVersion(const std::string &version)
	{
		m_version = version;
	}

	void setStatus(unsigned int status)
	{
		m_status = status;
	}

	unsigned int getStatus() const
	{
		return m_status;
	}

	void setReason(const char *reason)
	{
		const std::string s(reason);
		setReason(s);
	}

	void setStatusAndReason(unsigned int status,
			const std::string &reason)
	{
		setStatus(status);
		setReason(reason);
	}

	void setStatusAndReason(unsigned int status);

	void setReason(const std::string &reason)
	{
		m_reason = reason;
	}

	void setChunkedTransferEncoding(bool enable)
	{
		if (enable) {
			throw Poco::NotImplementedException(
				"chunked encoding is currently unsupported");
		}
	}

	std::ostream &send();

	bool sent() const
	{
		return m_headersSent;
	}

	void sendBuffer(const void *b, std::size_t blen);

	void setContentType(const std::string &type)
	{
		set("Content-Type", type);
	}

	bool hasContentLength() const
	{
		return m_contentLength >= 0;
	}

	void setContentLength(std::streamsize length)
	{
		m_contentLength = length;
	}

	void setKeepAlive(bool keepAlive)
	{
		m_keepAlive = keepAlive;
	}

	void setCredentials(const std::string & scheme,
			const std::string & authInfo)
	{
		set("Authorization", scheme + " " + authInfo);
	}

	/**
	 * Return true if a header of the given name exists
	 * in the response message.
	 */
	bool has(const std::string &name) const
	{
		return m_headers.find(name) != m_headers.end();
	}

	/**
	 * Set the header (name, value) into the response message.
	 */
	void set(const std::string &name,
			const std::string &value)
	{
		m_headers[name] = value;
	}

	/**
	 * Erase the header by its name from the response message.
	 */
	void erase(const std::string &name)
	{
		HeadersMap::iterator it = m_headers.find(name);

		if (it != m_headers.end())
			m_headers.erase(it);
	}

protected:
	void flushHeaders();
	bool noSendData() const;

private:
	bool m_noSendData;
	MongooseOutputStream m_output;
	Poco::NullOutputStream m_nullStream;
	bool m_headersSent;
	bool m_keepAlive;
	std::streamsize m_contentLength;
	std::string m_version;
	int m_status;
	std::string m_reason;
	HeadersMap m_headers;
};

}

#endif
