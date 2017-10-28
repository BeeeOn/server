#include <sstream>

#include <Poco/Exception.h>
#include <Poco/AutoPtr.h>
#include <Poco/TeeStream.h>
#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>
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
	m_timeout(0, 1000),
	m_logStream(logger(), Poco::Message::PRIO_DEBUG)
{
}

void LegacyGatewayRPC::setResponseTimeout(const Timespan &timeout)
{
	if (timeout < 0)
		throw InvalidArgumentException("response timeout must be non-negative");

	m_timeout = timeout;
}

void LegacyGatewayRPC::sendListen(
		const ResultCall &resultCall,
		const Gateway &gateway,
		const Poco::Timespan &duration)
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
	GatewayRPCResult::Ptr result = parseResponse(receiveResponse());

	m_executor->invoke([resultCall, result]()
	{
		resultCall(result);
	});
}

void LegacyGatewayRPC::unpairDevice(
		const ResultCall &resultCall,
		const Gateway &gateway,
		const Device &device)
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
	GatewayRPCResult::Ptr result = parseResponse(receiveResponse());

	m_executor->invoke([resultCall, result]()
	{
		resultCall(result);
	});
}

void LegacyGatewayRPC::pingGateway(
		const ResultCall &resultCall,
		const Gateway &gateway)
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
	GatewayRPCResult::Ptr result = parseResponse(receiveResponse());

	m_executor->invoke([resultCall, result]()
	{
		resultCall(result);
	});
}

void LegacyGatewayRPC::updateActor(
		const ResultCall &resultCall,
		const Gateway &gateway,
		const Device &device,
		const ModuleInfo &module,
		double value,
		const Timespan &timeout)
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

		attributes.clear();
		attributes.addAttribute("", "", "type", "", module.id().toString());
		xmlWriter.startElement("", "value", "", attributes);
		xmlWriter.characters(to_string(value));
		xmlWriter.endElement("", "value", "");

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
	GatewayRPCResult::Ptr result = parseResponse(receiveResponse());

	m_executor->invoke([resultCall, result]()
	{
		resultCall(result);
	});
}

string LegacyGatewayRPC::receiveResponse()
{
	stringstream response;
	Timestamp started;

	while (started.elapsed() < m_timeout.totalMicroseconds()) {
		StreamCopier::copyStreamUnbuffered(m_connector->receive(), response);

		const string &content = response.str();
		const size_t endTag = content.rfind("</reply>");

		if (endTag == string::npos)
			continue;

		m_connector->close();

		const size_t expectedEnd = endTag + string("</reply>").size();
		if (expectedEnd < content.size()) {
			logger().warning(
				"unexpected content after </replay>: "
				+ to_string(content.size() - expectedEnd),
				__FILE__, __LINE__);

			return response.str().substr(0, expectedEnd + 1);
		}

		break;
	}

	return response.str();
}

GatewayRPCResult::Ptr LegacyGatewayRPC::parseResponse(const string &response)
{
	GatewayRPCResult::Ptr result = new GatewayRPCResult;

	if (response.length() == 0) {
		result->setStatus(GatewayRPCResult::Status::TIMEOUT);
		return result;
	}

	if(logger().debug())
		logger().debug(response);

	string errorCode;

	try {
		AutoPtr<XML::Document> doc = m_xmlParser.parse(response);
		errorCode = doc->documentElement()->getAttribute("errorCode");
	} catch (const Exception &e) {
		logger().log(e);
		result->setStatus(GatewayRPCResult::Status::FAILED);
		return result;
	}

	if (errorCode == "0")
		result->setStatus(GatewayRPCResult::Status::SUCCESS);
	else if (errorCode == "1" || errorCode == "2")
		result->setStatus(GatewayRPCResult::Status::NOT_CONNECTED);
	else
		result->setStatus(GatewayRPCResult::Status::FAILED);

	return result;
}

BEEEON_OBJECT_BEGIN(BeeeOn, LegacyGatewayRPC)
BEEEON_OBJECT_CASTABLE(GatewayRPC)
BEEEON_OBJECT_REF("rpcConnector", &LegacyGatewayRPC::setRPCConnector)
BEEEON_OBJECT_REF("asyncExecutor", &LegacyGatewayRPC::setAsyncExecutor)
BEEEON_OBJECT_TIME("responseTimeout", &LegacyGatewayRPC::setResponseTimeout)
BEEEON_OBJECT_END(BeeeOn, LegacyGatewayRPC)
