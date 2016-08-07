#ifndef BEEEON_MONGOOSE_REQUEST_H
#define BEEEON_MONGOOSE_REQUEST_H

#include <iostream>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetException.h>
#include <Poco/String.h>
#include <Poco/MemoryStream.h>
#include <mongoose.h>

#include "mongoose/MongooseResponse.h"

namespace BeeeOn {

/**
 * Represents a mongoose-specific HTTP request. It can be used instead
 * of the Poco HTTPServerRequest (there is an early-binding relation
 * by utilizing C++ templates).
 */
class MongooseRequest {
public:
	MongooseRequest(const Poco::Net::SocketAddress &clientAddress,
			struct http_message *msg,
			MongooseResponse &resp):
		m_address(clientAddress),
		m_message(msg),
		m_input(msg->body.p, msg->body.len),
		m_response(resp),
		m_method(msg->method.p, msg->method.len),
		m_uri(msg->uri.p, msg->uri.len),
		m_version(msg->proto.p, msg->proto.len)
	{
	}

	const Poco::Net::SocketAddress &clientAddress() const
	{
		return m_address;
	}

	std::istream &stream()
	{
		return m_input;
	}

	MongooseResponse &response() const
	{
		return m_response;
	}

	const std::string &getMethod() const
	{
		return m_method;
	}

	const std::string &getURI() const
	{
		return m_uri;
	}

	const std::string &getVersion() const
	{
		return m_version;
	}

	std::streamsize getContentLength() const
	{
		return std::stoi(get("Content-Length", "-1"));
	}

	const std::string getContentType() const
	{
		return get("Content-Type", "");
	}

	bool hasContentLength() const
	{
		return has("Content-Length");
	}

	bool getKeepAlive() const
	{
		const std::string &keepAlive = get("Connection", "close");
		return !Poco::icompare(keepAlive, "keep-alive");
	}

	bool hasCredentials() const
	{
		return has("Authorization");
	}

	void getCredentials(std::string & scheme,
			std::string & authInfo) const
	{
		if (!has("Authorization"))
			throw Poco::Net::NotAuthenticatedException("missing Authorization header");

		const std::string &auth(get("Authorization"));
		std::size_t pos = auth.find(" ");

		if (pos == std::string::npos)
			throw Poco::Net::NotAuthenticatedException("unexpected Authorization header content");

		scheme = auth.substr(0, pos);
		authInfo = auth.substr(pos + 1);
	}

	/**
	 * Return true if a header of the given name exists
	 * in the request message.
	 */
	bool has(const std::string name) const
	{
		return has(name.c_str());
	}

	bool has(const char *name) const
	{
		return mg_get_http_header(m_message, name) != NULL;
	}

	/**
	 * Return value of a message header specified by name.
	 */
	const std::string get(const std::string &name) const
	{
		struct mg_str *value;
		value = mg_get_http_header(m_message, name.c_str());

		if (value == NULL)
			return std::string();

		return std::string(value->p, value->len);
	}

	/**
	 * Return value of a message header specified by name.
	 * If such header does not exist return defValue.
	 */
	const std::string get(const std::string &name,
			const std::string defValue) const
	{
		return has(name)? get(name) : defValue;
	}

	/**
	 * Return value of a message header specified by name.
	 */
	const std::string operator[](const std::string &name) const
	{
		return get(name);
	}

	/**
	 * Return count of available message headers.
	 */
	std::size_t size()
	{
		std::size_t count = 0;

		for (int i = 0; i < MG_MAX_HTTP_HEADERS; ++i) {
			if (m_message->header_names[i].len > 0)
				count += 1;
		}

		return count;
	}

private:
	const Poco::Net::SocketAddress &m_address;
	struct http_message *m_message;
	Poco::MemoryInputStream m_input;
	MongooseResponse &m_response;
	const std::string m_method;
	const std::string m_uri;
	const std::string m_version;
};

}

#endif
