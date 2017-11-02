#include <Poco/Exception.h>

#include "server/TestingHTTPServerResponse.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

TestingHTTPServerResponse::TestingHTTPServerResponse():
	m_continueSent(false),
	m_redirected(false),
	m_authenticationRequired(false)
{
}

void TestingHTTPServerResponse::sendContinue()
{
	m_continueSent = true;
}

bool TestingHTTPServerResponse::continueSent() const
{
	return m_continueSent;
}

ostream &TestingHTTPServerResponse::send()
{
	return m_stream;
}

ostringstream &TestingHTTPServerResponse::stringstream()
{
	return m_stream;
}

void TestingHTTPServerResponse::sendFile(const string &path, const string &mediaType)
{
	throw NotImplementedException(__func__);
}

void TestingHTTPServerResponse::sendBuffer(const void *buffer, size_t length)
{
	m_stream.write((const char *) buffer, length);
}

void TestingHTTPServerResponse::redirect(const string &uri, HTTPStatus status)
{
	m_redirectURI = uri;
	m_redirectStatus = status;
	m_redirected = true;
}

string TestingHTTPServerResponse::redirectURI() const
{
	return m_redirectURI;
}

TestingHTTPServerResponse::HTTPStatus TestingHTTPServerResponse::redirectStatus() const
{
	return m_redirectStatus;
}

bool TestingHTTPServerResponse::redirected() const
{
	return m_redirected;
}

void TestingHTTPServerResponse::requireAuthentication(const string &realm)
{
	m_authenticationRealm = realm;
	m_authenticationRequired = true;
}

bool TestingHTTPServerResponse::authenticationRequired() const
{
	return m_authenticationRequired;
}

string TestingHTTPServerResponse::authenticationRealm() const
{
	return m_authenticationRealm;
}

bool TestingHTTPServerResponse::sent() const
{
	return !m_stream.str().empty()
		|| m_continueSent
		|| m_redirected
		|| m_authenticationRequired;
}
