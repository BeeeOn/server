#include <cmath>

#include <Poco/Logger.h>
#include <Poco/NumberParser.h>
#include <Poco/SAX/Attributes.h>

#include "util/TypesSAXHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

TypesSAXHandler::TypesSAXHandler()
{
	m_typeExpect.insert("name");
	m_typeExpect.insert("unit");
	m_typeExpect.insert("range");

	m_leafElements.insert("name");
	m_leafElements.insert("unit");
}

TypesSAXHandler::~TypesSAXHandler()
{
}

void TypesSAXHandler::startElement(
		const SAXElement &element,
		const Attributes &attrList)
{
	if (logger().trace()) {
		logger().trace(string(__func__) + ": " + element.qName,
				__FILE__, __LINE__);
	}

	if (isPathFromRoot("types", "type")) {
		XMLString id;
		if (!getAndTrimAttribute(attrList, "id", id))
			error("missing attribute id");

		if (id.empty())
			error("id value is empty");

		if (logger().debug())
			logger().debug("parsing type of @id " + id, __FILE__, __LINE__);

		m_temp.setId(TypeInfoID::parse(id));
		m_temp.setName("");
		m_temp.setUnit("");
		m_temp.setRange(TypeInfo::Range());
	}

	if (isPathFromRoot("types", "type", "range")) {
		if (m_temp.range().isValid())
			error("duplicate element range");

		double min = NAN;
		double max = NAN;
		double step = NAN;

		(void) getAttributeAsDouble(attrList, "min", min);
		(void) getAttributeAsDouble(attrList, "max", max);
		(void) getAttributeAsDouble(attrList, "step", step);

		TypeInfo::Range range(min, max, step);
		m_temp.setRange(range);

		if (logger().debug()) {
			logger().debug("parsing range "
				"@min=" + to_string(min)
				+ " @max=" + to_string(max)
				+ " @step=" + to_string(step),
				__FILE__, __LINE__);
		}
	}
}

void TypesSAXHandler::endElement(const SAXElement &element)
{
	if (logger().trace()) {
		logger().trace(string(__func__) + ": " + element.qName,
				__FILE__, __LINE__);
	}

	if (isPathFromRoot("types", "type", "name")) {
		m_temp.setName(element.content);

		if (logger().debug())
			logger().debug("parsing @name " + element.content, __FILE__, __LINE__);
	}

	if (isPathFromRoot("types", "type", "unit")) {
		m_temp.setUnit(element.content);

		if (logger().debug())
			logger().debug("parsing unit of value " + element.content, __FILE__, __LINE__);
	}

	if (isPathFromRoot("types", "type")) {
		if (m_temp.name().empty())
			error("missing name of type " + m_temp);

		if (m_result.find(m_temp) != m_result.end())
			error("type of id " + m_temp.id().toString() + " already exists");

		if (logger().debug())
			logger().debug("inserting type " + m_temp, __FILE__, __LINE__);

		m_result.insert(m_temp);
	}
}

bool TypesSAXHandler::expectElement(const SAXElement &element) const
{
	if (current().isRoot())
		return element.qName == "types";

	if (isPathFromRoot("types"))
		return element.qName == "type";

	if (isPathFromRoot("types", "type"))
		return m_typeExpect
			.find(element.qName) != m_typeExpect.end();

	return false;
}

bool TypesSAXHandler::expectContent(int length) const
{
	if (m_leafElements.find(current().qName) != m_leafElements.end())
		return true;

	return length == 0;
}

void TypesSAXHandler::preprocessContent(const XMLChar c[], int &start, int &length)
{
	trimContent(c, start, length);
}
