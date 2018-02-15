#include <string>

#include <Poco/Exception.h>
#include <Poco/XML/XMLWriter.h>

#include "xmlui/XmlValueConsumer.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

XmlValueConsumer::XmlValueConsumer(Poco::XML::XMLWriter &writer):
	m_writer(writer),
	m_info(NULL)
{
}

void XmlValueConsumer::begin(const TypeInfo &info)
{
	m_info = &info;
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

void XmlValueConsumer::end()
{
	m_info = NULL;
}
