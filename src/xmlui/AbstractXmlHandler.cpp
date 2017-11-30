#include <Poco/Exception.h>
#include <Poco/Net/NetException.h>
#include <Poco/SAX/AttributesImpl.h>

#include "xmlui/AbstractXmlHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

AbstractXmlHandler::AbstractXmlHandler(
		const string &ns,
		const StreamSocket &socket,
		const AutoPtr<Document> input):
	BeeeOn::XmlRequestHandler(socket, input),
	m_ns(ns)
{
}

void AbstractXmlHandler::handleInput()
{
	try {
		handleInputImpl();
	}
	catch (const NotFoundException &e) {
		logger().log(e, __FILE__, __LINE__);
		resultNotFound();
	}
	catch (const NotAuthenticatedException &e) {
		logger().log(e, __FILE__, __LINE__);
		resultNotAuthenticated();
	}
	catch (const InvalidAccessException &e) {
		logger().log(e, __FILE__, __LINE__);
		resultForbidden();
	}
	catch (const InvalidArgumentException &e) {
		logger().log(e, __FILE__, __LINE__);
		resultInvalidInput();
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		resultUnexpected();
	}
	catch (const exception &e) {
		logger().critical(e.what(), __FILE__, __LINE__);
		resultUnexpected();
	}
	catch (const char *s) {
		logger().critical(s, __FILE__, __LINE__);
		resultUnexpected();
	}
	catch (...) {
		logger().critical("unknown error, cought '...'",
				__FILE__, __LINE__);
		resultUnexpected();
	}
}

void AbstractXmlHandler::deriveType(Poco::XML::AttributesImpl &attrs)
{
	const Element *root = m_input->documentElement();
	attrs.addAttribute("", "type", "type", "",
			root->getAttribute("type"));
}

void AbstractXmlHandler::resultSimple(AttributesImpl &attrs,
		const std::string &result)
{
	attrs.addAttribute("", "ns", "ns", "", m_ns);
	attrs.addAttribute("", "version", "version", "", "1.0.0");
	attrs.addAttribute("", "result", "result", "", result);
	deriveType(attrs);

	m_output.emptyElement("", "response", "response", attrs);
}

void AbstractXmlHandler::resultSuccess()
{
	AttributesImpl attrs;
	resultSimple(attrs, "ok");
}

void AbstractXmlHandler::resultDataStart()
{
	AttributesImpl attrs;
	attrs.addAttribute("", "ns", "ns", "", m_ns);
	attrs.addAttribute("", "version", "version", "", "1.0.0");
	attrs.addAttribute("", "result", "result", "", "data");
	deriveType(attrs);

	m_output.startElement("", "response", "response", attrs);
}

void AbstractXmlHandler::resultDataEnd()
{
	m_output.endElement("", "response", "response");
}

void AbstractXmlHandler::resultNotAuthenticated()
{
	resultError("20"); // resign
}

void AbstractXmlHandler::resultForbidden()
{
	resultError("1001");
}

void AbstractXmlHandler::resultUnexpected()
{
	resultError("998"); // unknown
}

void AbstractXmlHandler::resultInvalidInput()
{
	resultError("998"); // FIXME
}


void AbstractXmlHandler::resultNotFound()
{
	resultError("13");
}

void AbstractXmlHandler::resultAlreadyAssigned()
{
	resultError("31");
}

void AbstractXmlHandler::resultNotOwned()
{
	resultError("6");
}

void AbstractXmlHandler::resultError(const string &code)
{
	AttributesImpl attrs;
	attrs.addAttribute("", "errcode", "errcode", "", code);
	resultSimple(attrs, "error");
}

AbstractXmlHandlerResolver::AbstractXmlHandlerResolver(
		const string &ns):
	m_ns(ns)
{
}

bool AbstractXmlHandlerResolver::canHandle(
		const Document &doc)
{
	Element *root = doc.documentElement();
	if (root == NULL)
		return false;

	return canHandle(*root);
}

bool AbstractXmlHandlerResolver::canHandle(
		const Element &root)
{
	if (root.localName() != "request")
		return false;

	if (root.getAttribute("ns") != m_ns)
		return false;

	return true;

}
