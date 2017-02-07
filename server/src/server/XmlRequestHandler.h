#ifndef BEEEON_XML_REQUEST_HANDLER_H
#define BEEEON_XML_REQUEST_HANDLER_H

#include <unistd.h>

#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <Poco/Exception.h>
#include <Poco/TeeStream.h>
#include <Poco/LogStream.h>

#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>

#include <Poco/DOM/Document.h>
#include <Poco/XML/XMLWriter.h>

#include "util/SecureXmlParser.h"
#include "util/Loggable.h"
#include "di/InjectorTarget.h"
#include "server/SocketServer.h"

namespace BeeeOn {

class XmlRequestHandler : public Poco::Net::TCPServerConnection,
			public Loggable {
public:
	XmlRequestHandler(
		const Poco::Net::StreamSocket &socket,
		const Poco::AutoPtr<Poco::XML::Document> input);
	void run() override;
	virtual void handleInput() = 0;

protected:
	const Poco::AutoPtr<Poco::XML::Document> m_input;
	Poco::Net::SocketOutputStream m_socketStream;
	Poco::Logger &m_logStreamLogger;
	Poco::LogStream m_logStream;
	Poco::TeeOutputStream m_stream;
	Poco::XML::XMLWriter m_output;
};

class XmlRequestHandlerResolver : public BeeeOn::AbstractInjectorTarget,
		public Loggable {
public:
	virtual bool canHandle(const Poco::XML::Document &input) = 0;
	virtual XmlRequestHandler *createHandler(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input) = 0;
};

class XmlRequestHandlerFactory :
	public SocketServerConnectionFactory,
	public Loggable {
private:
	void init();
public:
	XmlRequestHandlerFactory();
	XmlRequestHandlerFactory(std::size_t maxLength);

	Poco::Net::TCPServerConnection *createConnection(
			const Poco::Net::StreamSocket &socket);

	Poco::AutoPtr<Poco::XML::Document> parseDocument(
			Poco::Net::StreamSocket &socket);

	XmlRequestHandler *resolveRequest(
			const Poco::Net::StreamSocket &socket,
			const Poco::AutoPtr<Poco::XML::Document> input);

	void registerResolver(XmlRequestHandlerResolver *resolver);

	void setMaxLength(int length)
	{
		if (length <= 0) {
			throw Poco::InvalidArgumentException(
				"invalid max buffer length");
		}

		m_maxLength = length;
	}

private:
	std::vector<XmlRequestHandlerResolver *> m_resolvers;
	BeeeOn::SecureXmlParser m_parser;
	std::size_t m_maxLength;
};

}

#endif
