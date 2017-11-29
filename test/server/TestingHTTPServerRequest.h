#ifndef BEEEON_TESTING_HTTP_SERVER_REQUEST_H
#define BEEEON_TESTING_HTTP_SERVER_REQUEST_H

#include <sstream>

#include <Poco/Version.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/SocketAddress.h>

namespace BeeeOn {

class TestingHTTPServerRequest : public Poco::Net::HTTPServerRequest {
public:
	TestingHTTPServerRequest(Poco::Net::HTTPServerResponse &response);

	std::istream &stream() override;
	std::istringstream &stringstream();

#if POCO_VERSION < 0x01080000
	void setExpectContinue(bool expect);
	bool expectContinue() const override;
#endif

#if POCO_VERSION >= 0x01080000
	bool secure() const override;
#endif

	void setClientAddress(const Poco::Net::SocketAddress &address);
	const Poco::Net::SocketAddress &clientAddress() const override;

	void setServerAddress(const Poco::Net::SocketAddress &address);
	const Poco::Net::SocketAddress &serverAddress() const override;

	void setServerParams(Poco::Net::HTTPServerParams::Ptr params);
	const Poco::Net::HTTPServerParams &serverParams() const override;

	Poco::Net::HTTPServerResponse &response() const override;

private:
#if POCO_VERSION < 0x01080000
	bool m_expectContinue;
#endif
	std::istringstream m_stream;
	Poco::Net::SocketAddress m_clientAddress;
	Poco::Net::SocketAddress m_serverAddress;
	Poco::Net::HTTPServerParams::Ptr m_serverParams;
	Poco::Net::HTTPServerResponse &m_response;
};

}

#endif
