#include <Poco/Net/HTTPResponse.h>

#include "mongoose/MongooseResponse.h"
#include "Debug.h"

using namespace std;
using namespace BeeeOn;

const string MongooseResponse::HTTP_1_0 = "HTTP/1.0";
const string MongooseResponse::HTTP_1_1 = "HTTP/1.1";

void MongooseResponse::redirect(const string uri, int status)
{
	TRACE_METHOD();

	setStatusAndReason(HTTP_FOUND);
	setChunkedTransferEncoding(false);
	set("Location", uri);
	flushHeaders();
}

void MongooseResponse::requireAuthentication(
		const string &realm, const string &scheme)
{
	TRACE_METHOD();

	setStatusAndReason(HTTP_UNAUTHORIZED);
	set("WWW-Authenticate", scheme + " realm=\"" + realm + "\"");
	flushHeaders();
}

void MongooseResponse::sendBuffer(const void *b, size_t blen)
{
	TRACE_METHOD();

	setContentLength(blen);
	setChunkedTransferEncoding(false);
	flushHeaders();

	if (noSendData())
		return;

	m_output.write((char *) b, blen);
	m_output.flush();
}

bool MongooseResponse::noSendData() const
{
	return m_noSendData || m_status < HTTP_OK;
}

ostream &MongooseResponse::send()
{
	TRACE_METHOD();
	flushHeaders();

	if (noSendData())
		return m_nullStream;

	return m_output;
}

/**
 * Flush all headers and prepare for writing body.
 * The method is not thread-safe (seems unreasonable).
 */
void MongooseResponse::flushHeaders()
{
	TRACE_METHOD();

	if (m_headersSent)
		return;

	HeadersMap::const_iterator it;
	streamsize length = m_contentLength < 0 ? 0 : m_contentLength;

	m_output << m_version << " "
		 << m_status << " "
		 << m_reason << "\r\n";

	m_output << "Content-Length: " << length << "\r\n";

	for (it = m_headers.begin(); it != m_headers.end(); ++it)
		m_output << it->first << ": " << it->second << "\r\n";

	m_output << "\r\n";

	m_headersSent = true;
}

void MongooseResponse::setStatusAndReason(unsigned int status)
{
	setStatus(status);
	setReason(Poco::Net::HTTPResponse::getReasonForStatus(
			(Poco::Net::HTTPResponse::HTTPStatus) status));
}
