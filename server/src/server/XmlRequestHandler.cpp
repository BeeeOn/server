#include <Poco/Exception.h>
#include <Poco/FIFOBuffer.h>
#include <Poco/Net/NetException.h>

#include "Debug.h"
#include "server/XmlRequestHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;

XmlRequestHandler::XmlRequestHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input):
	TCPServerConnection(socket),
	m_logger(LOGGER_CLASS(this)),
	m_input(input),
	m_stream(socket),
	m_output(m_stream, XMLWriter::Options::CANONICAL_XML)
{
}

void XmlRequestHandler::run()
{
	_TRACE_METHOD(m_logger);

	try {
		m_output.startDocument();
		handleInput();
		m_output.endDocument();
	} catch (const Exception &e) {
		m_logger.log(e, __FILE__, __LINE__);
	}
}

XmlRequestHandlerFactory::XmlRequestHandlerFactory():
	m_logger(LOGGER_CLASS(this)),
	m_maxLength(4096)
{
	init();
}

XmlRequestHandlerFactory::XmlRequestHandlerFactory(size_t maxLength):
	m_logger(LOGGER_CLASS(this)),
	m_maxLength(maxLength)
{
	init();
}

void XmlRequestHandlerFactory::init()
{
	injector<XmlRequestHandlerFactory, XmlRequestHandlerResolver>
		("resolvers", &XmlRequestHandlerFactory::registerResolver);
	numberInjector("maxBuffer", (NumberSetter)
			&XmlRequestHandlerFactory::setMaxLength);
}

TCPServerConnection *XmlRequestHandlerFactory::createConnection(
	const StreamSocket &socket)
{
	try {
		StreamSocket readableSocket(socket);
		return resolveRequest(socket, parseDocument(readableSocket));
	}
	catch (const Exception &e) {
		m_logger.log(e, __FILE__, __LINE__);
	}
	catch (const exception &e) {
		m_logger.critical(e.what(), __FILE__, __LINE__);
	}

	return NULL;
}

XmlRequestHandler *XmlRequestHandlerFactory::resolveRequest(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	for (auto resolver : m_resolvers) {
		if (!resolver->canHandle(*input))
			continue;

		return resolver->createHandler(socket, input);
	}

	m_logger.critical("could not resolve handler", __FILE__, __LINE__);
	return NULL;
}

void XmlRequestHandlerFactory::registerResolver(XmlRequestHandlerResolver *resolver)
{
	m_resolvers.push_back(resolver);
}

AutoPtr<Document> XmlRequestHandlerFactory::parseDocument(
		StreamSocket &socket)
{
	FIFOBuffer buffer(m_maxLength > 1024? m_maxLength : 1024);
	string xml;
	int ret = 0;

	do {
		buffer.drain(ret);
		ret = socket.receiveBytes(buffer);
		if (ret <= 0) {
			throw ConnectionResetException(
				socket.peerAddress().toString());
		}

		xml += string(buffer.begin(), buffer.used());
	} while(buffer.isFull() || xml.size() >= m_maxLength);

	size_t length = xml.size() > m_maxLength? m_maxLength : xml.size();
	return m_parser.parse(xml.c_str(), length);
}

BEEEON_OBJECT(XmlRequestHandlerFactory, BeeeOn::XmlRequestHandlerFactory)
