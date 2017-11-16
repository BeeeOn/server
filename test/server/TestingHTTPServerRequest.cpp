#include <Poco/Net/HTTPServerResponse.h>

#include "server/TestingHTTPServerRequest.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

TestingHTTPServerRequest::TestingHTTPServerRequest(HTTPServerResponse &response):
	m_expectContinue(false),
	m_response(response)
{
}

istream &TestingHTTPServerRequest::stream()
{
	return m_stream;
}

istringstream &TestingHTTPServerRequest::stringstream()
{
	return m_stream;
}

void TestingHTTPServerRequest::setExpectContinue(bool expect)
{
	m_expectContinue = expect;
}

bool TestingHTTPServerRequest::expectContinue() const
{
	return m_expectContinue;
}

void TestingHTTPServerRequest::setClientAddress(const SocketAddress &address)
{
	m_clientAddress = address;
}

const SocketAddress &TestingHTTPServerRequest::clientAddress() const
{
	return m_clientAddress;
}

void TestingHTTPServerRequest::setServerAddress(const SocketAddress &address)
{
	m_serverAddress = address;
}

const SocketAddress &TestingHTTPServerRequest::serverAddress() const
{
	return m_serverAddress;
}

void TestingHTTPServerRequest::setServerParams(HTTPServerParams::Ptr params)
{
	m_serverParams = params;
}

const HTTPServerParams &TestingHTTPServerRequest::serverParams() const
{
	return *m_serverParams;
}

HTTPServerResponse &TestingHTTPServerRequest::response() const
{
	return m_response;
}
