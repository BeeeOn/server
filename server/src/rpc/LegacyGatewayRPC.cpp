#include <sstream>

#include <Poco/Exception.h>
#include <Poco/AutoPtr.h>
#include <Poco/TeeStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/SAX/AttributesImpl.h>
#include <Poco/XML/XMLWriter.h>

#include "di/Injectable.h"
#include "rpc/LegacyGatewayRPC.h"
#include "rpc/RPCConnector.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

LegacyGatewayRPC::LegacyGatewayRPC():
	m_logStream(logger(), Poco::Message::PRIO_DEBUG)
{
}

void LegacyGatewayRPC::sendListen(const Gateway &gateway)
{
	Mutex::ScopedLock guard(m_lock);
	m_connector->open();

	TeeOutputStream outStream(m_connector->send());
	outStream.addStream(m_logStream);

	XMLWriter xmlWriter(outStream, XMLWriter::CANONICAL_XML);
	AttributesImpl attributes;

	try {
		xmlWriter.startDocument();
		attributes.addAttribute("", "", "type", "", "listen");
		xmlWriter.startElement("", "request", "", attributes);

		attributes.clear();
		attributes.addAttribute("", "", "id", "", gateway.id().toString());
		xmlWriter.startElement("", "adapter", "", attributes);
		xmlWriter.endElement("", "adapter", "");

		xmlWriter.endElement("", "request", "");
		xmlWriter.endDocument();

		m_connector->flush();
	}
	catch (const Exception &e) {
		m_logStream << endl;
		m_connector->close();
		e.rethrow();
	}

	m_logStream << endl;
	parseResponse(receiveResponse());
}

void LegacyGatewayRPC::unpairDevice(const Gateway &gateway, const Device &device)
{
	Mutex::ScopedLock guard(m_lock);
	m_connector->open();

	TeeOutputStream outStream(m_connector->send());
	outStream.addStream(m_logStream);

	XMLWriter xmlWriter(outStream, XMLWriter::CANONICAL_XML);
	AttributesImpl attributes;

	try {
		xmlWriter.startDocument();
		attributes.addAttribute("", "", "type", "", "delete");
		xmlWriter.startElement("", "request", "", attributes);

		attributes.clear();
		attributes.addAttribute("", "", "id", "", device.id().toString());
		attributes.addAttribute("", "", "onAdapter", "", gateway.id().toString());
		xmlWriter.startElement("", "sensor", "", attributes);
		xmlWriter.endElement("", "sensor", "");

		xmlWriter.endElement("", "request", "");
		xmlWriter.endDocument();

		m_connector->flush();
	}
	catch (const Exception &e) {
		m_logStream << endl;
		m_connector->close();
		e.rethrow();
	}

	m_logStream << endl;
	parseResponse(receiveResponse());
}

void LegacyGatewayRPC::pingGateway(const Gateway &gateway)
{
	Mutex::ScopedLock guard(m_lock);
	m_connector->open();

	TeeOutputStream outStream(m_connector->send());
	outStream.addStream(m_logStream);

	XMLWriter xmlWriter(outStream, XMLWriter::CANONICAL_XML);
	AttributesImpl attributes;

	try {
		xmlWriter.startDocument();
		attributes.addAttribute("", "", "type", "", "ping");
		xmlWriter.startElement("", "request", "", attributes);

		attributes.clear();
		attributes.addAttribute("", "", "id", "", gateway.id().toString());
		xmlWriter.startElement("", "adapter", "", attributes);
		xmlWriter.endElement("", "adapter", "");

		xmlWriter.endElement("", "request", "");
		xmlWriter.endDocument();

		m_connector->flush();
	}
	catch (const Exception &e) {
		m_logStream << endl;
		m_connector->close();
		e.rethrow();
	}

	m_logStream << endl;
	parseResponse(receiveResponse());
}

string LegacyGatewayRPC::receiveResponse()
{
	stringstream response;
	StreamCopier::copyStreamUnbuffered(m_connector->receive(), response);
	m_connector->close();

	return response.str();
}

void LegacyGatewayRPC::parseResponse(const string &response)
{
	if (response.length() == 0)
		throw InvalidArgumentException("response is empty.");

	if(logger().debug())
		logger().debug(response);

	AutoPtr<XML::Document> doc = m_xmlParser.parse(response);
	string errorCode = doc->documentElement()->getAttribute("errorCode");

	if (errorCode == "0")
		return;
	else if (errorCode == "1")
		throw IllegalStateException("gateway was never connected to the server");
	else if (errorCode == "2")
		throw IllegalStateException("connection with gateway is broken");
	else if (errorCode == "3")
		throw IllegalStateException("error sending data to the gateway");
	else if (errorCode == "4")
		throw IllegalStateException("PING response has not arrived");
	else if (errorCode == "5")
		throw IllegalStateException("message is not supported by the gateway protocol");
	else
		throw IllegalStateException("illegal error code: " + errorCode);
}

BEEEON_OBJECT_BEGIN(BeeeOn, LegacyGatewayRPC)
BEEEON_OBJECT_CASTABLE(GatewayRPC)
BEEEON_OBJECT_REF("rpcConnector", &LegacyGatewayRPC::setRPCConnector)
BEEEON_OBJECT_END(BeeeOn, LegacyGatewayRPC)
