#include <sstream>

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <Poco/FIFOBuffer.h>
#include <Poco/Net/NetException.h>
#include <Poco/DOM/DOMWriter.h>

#include "di/Injectable.h"
#include "server/XmlRequestHandler.h"
#include "server/FailingTCPServerConnection.h"
#include "util/ThreadNamer.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;

XmlRequestHandler::XmlRequestHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input):
	TCPServerConnection(socket),
	m_input(input),
	m_socketStream(socket),
	m_logStream(Loggable::forInstance(this), Message::PRIO_DEBUG),
	m_stream(m_socketStream),
	m_output(m_stream, XMLWriter::Options::CANONICAL_XML)
{
	m_stream.addStream(m_logStream);
}

void XmlRequestHandler::setStarted(const Clock &started)
{
	m_started = started;
}

const Clock &XmlRequestHandler::started() const
{
	return m_started;
}

void XmlRequestHandler::run()
{
	ThreadNamer namer("xmlui-" + socket().peerAddress().toString());

	try {
		m_output.startDocument();
		handleInput();
		m_output.endDocument();
		m_logStream << std::endl;
	} catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
	}

	if (logger().information()) {
		logger().information("duration: "
				+ to_string(m_started.elapsed()) + "us",
				__FILE__, __LINE__);
	}
}

XmlRequestHandlerFactory::XmlRequestHandlerFactory():
	m_maxLength(4096)
{
	init();
}

XmlRequestHandlerFactory::XmlRequestHandlerFactory(size_t maxLength):
	m_maxLength(maxLength)
{
	init();
}

void XmlRequestHandlerFactory::init()
{
}

TCPServerConnection *XmlRequestHandlerFactory::createConnection(
	const StreamSocket &socket)
{
	const Clock started;

	try {
		StreamSocket readableSocket(socket);

		if (logger().information()) {
			const SocketAddress &peer = readableSocket.peerAddress();
			logger().information("accepting client "
				+ peer.toString()
				+ (readableSocket.secure()? " (secure)" : " (insecure)"),
				__FILE__, __LINE__);
		}

		XmlRequestHandler *handler = resolveRequest(socket, parseDocument(readableSocket));
		handler->setStarted(started);
		return handler;
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
	}
	catch (const exception &e) {
		logger().critical(e.what(), __FILE__, __LINE__);
	}

	return new FailingTCPServerConnection(socket);
}

static string documentToString(const AutoPtr<Document> doc)
{
	DOMWriter writer;
	ostringstream s;
	writer.writeNode(s, doc);
	return s.str();
}

XmlRequestHandler *XmlRequestHandlerFactory::resolveRequest(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	if (logger().debug()) {
		logger().debug("document: " + documentToString(input),
				__FILE__, __LINE__);
	}

	for (auto resolver : m_resolvers) {
		if (!resolver->canHandle(*input))
			continue;

		return resolver->createHandler(socket, input);
	}

	logger().critical("could not resolve handler", __FILE__, __LINE__);
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
	} while(buffer.isFull() && xml.size() < m_maxLength);

	if (xml.size() > m_maxLength) {
		logger().notice("too long input: "
				+ to_string(xml.size()) + " B",
				__FILE__, __LINE__);
	}

	size_t length = xml.size() > m_maxLength? m_maxLength : xml.size();
	return m_parser.parse(xml.c_str(), length);
}

BEEEON_OBJECT_BEGIN(BeeeOn, XmlRequestHandlerFactory)
BEEEON_OBJECT_CASTABLE(SocketServerConnectionFactory)
BEEEON_OBJECT_PROPERTY("resolvers", &XmlRequestHandlerFactory::registerResolver)
BEEEON_OBJECT_PROPERTY("maxBuffer", &XmlRequestHandlerFactory::setMaxLength)
BEEEON_OBJECT_END(BeeeOn, XmlRequestHandlerFactory)
