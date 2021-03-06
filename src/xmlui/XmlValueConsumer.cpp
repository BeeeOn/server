#include <string>

#include <Poco/Exception.h>
#include <Poco/SAX/AttributesImpl.h>
#include <Poco/XML/XMLWriter.h>

#include "xmlui/XmlValueConsumer.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

XmlValueConsumer::XmlValueConsumer(Poco::XML::XMLWriter &writer):
	m_writer(writer),
	m_info(NULL),
	m_hasBegin(false)
{
}

bool XmlValueConsumer::hasBegin() const
{
	return m_hasBegin;
}

void XmlValueConsumer::begin(const TypeInfo &info)
{
	m_info = &info;

	AttributesImpl attrs;
	attrs.addAttribute("", "ns", "ns", "", "devices");
	attrs.addAttribute("", "version", "version", "", "1.0.0");
	attrs.addAttribute("", "result", "result", "", "data");
	attrs.addAttribute("", "type", "type", "", "getlog");
	m_writer.startElement("", "response", "response", attrs);

	m_hasBegin = true;
}

const TypeInfo &XmlValueConsumer::info() const
{
	if (m_info == NULL)
		throw IllegalStateException("info is missing, forgot to call begin()?");

	return *m_info;
}

void XmlValueConsumer::single(const ValueAt &v)
{
	m_writer.dataElement("", "row", "row", to_string(v.at().epochTime()) + " " + info().asString(v.value()));
}

void XmlValueConsumer::multiple(const vector<ValueAt> &v)
{
	if (v.empty())
		return;

	single(v.front());
}

void XmlValueConsumer::frequency(const ValueAt &v, size_t count)
{
	AttributesImpl attrs;
	attrs.addAttribute("", "freq", "freq", "", to_string(count));
	m_writer.startElement("", "row", "row", attrs);

	const auto &content = to_string(v.at().epochTime())
		+ " " + info().asString(v.value());

	m_writer.characters(content.c_str(), 0, content.size());
	m_writer.endElement("", "row", "row");
}

void XmlValueConsumer::end()
{
	m_writer.endElement("", "response", "response");
	m_info = NULL;
}
