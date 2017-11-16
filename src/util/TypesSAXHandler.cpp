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
	m_typeExpect.insert("values");
	m_typeExpect.insert("levels");

	m_valuesExpect.insert("value");

	m_levelsExpect.insert("level");

	m_leafElements.insert("name");
	m_leafElements.insert("unit");
	m_leafElements.insert("value");
	m_leafElements.insert("level");
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
		m_temp.setValues({});
		m_temp.setLevels({});
	}

	if (isPathFromRoot("types", "type", "range")) {
		if (m_temp.range().isValid())
			error("duplicate element range");

		if (!m_temp.values().empty())
			error("unexpected element range, values aready defined");

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

	if (isPathFromRoot("types", "type", "values")) {
		if (m_temp.range().isValid())
			error("unexpected element values, range already defined");
	}

	if (isPathFromRoot("types", "type", "values", "value")) {
		if (!getAttributeAsInt(attrList, "equals", m_lastValue))
			error("missing attribute equals");

		if (!m_values.emplace(m_lastValue, "").second)
			error("duplicate value with equals to " + to_string(m_lastValue));

		if (logger().debug())
			logger().debug("parsing value equals to " + to_string(m_lastValue), __FILE__, __LINE__);
	}

	if (isPathFromRoot("types", "type", "levels", "level")) {
		double min = NAN;
		double max = NAN;

		if (getAttributeAsDouble(attrList, "equals", min)) {
			m_level = TypeInfo::Level(min);
		}
		else if (getAttributeAsDouble(attrList, "min", min)) {
			if (getAttributeAsDouble(attrList, "max", max))
				m_level = TypeInfo::Level(min, max);
			else
				m_level = TypeInfo::Level(min, NAN);
		}
		else if (getAttributeAsDouble(attrList, "max", max)) {
			m_level = TypeInfo::Level(NAN, max);
		}
		else {
			error("missing attribute equals or attributes min and max");
		}

		XMLString attention;
		if (getAndTrimAttribute(attrList, "attention", attention)) {
			if (attention == "none")
				m_level.setAttention(TypeInfo::Level::NONE);
			else if (attention == "single")
				m_level.setAttention(TypeInfo::Level::SINGLE);
			else if (attention == "repeat")
				m_level.setAttention(TypeInfo::Level::REPEAT);
			else if (attention == "alert")
				m_level.setAttention(TypeInfo::Level::ALERT);
			else
				error("unexpected value of attribute attention: " + attention);
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

	if (isPathFromRoot("types", "type", "values", "value")) {
		m_values[m_lastValue] = element.content;

		if (logger().debug())
			logger().debug("parsing value " + to_string(m_lastValue) + ": " + element.content, __FILE__, __LINE__);
	}

	if (isPathFromRoot("types", "type", "values")) {
		m_temp.setValues(m_values);
		m_values.clear();

		if (logger().debug())
			logger().debug("parsing values " + to_string(m_temp.values().size()), __FILE__, __LINE__);
	}

	if (isPathFromRoot("types", "type", "levels", "level")) {
		if (!element.content.empty())
			m_level.setLabel(element.content);

		if (!m_levels.emplace(m_level).second)
			error("overlapping level: " + to_string(m_level.min()) + ".." + to_string(m_level.max()));

		if (logger().debug()) {
			logger().debug("parsing level "
				+ to_string(m_level.min())
				+ ".."
				+ to_string(m_level.max()),
				__FILE__, __LINE__);
		}
	}

	if (isPathFromRoot("types", "type", "levels")) {
		m_temp.setLevels(m_levels);
		m_levels.clear();

		if (logger().debug())
			logger().debug("parsing levels " + to_string(m_temp.levels().size()), __FILE__, __LINE__);
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

	if (isPathFromRoot("types", "type", "values"))
		return m_valuesExpect
			.find(element.qName) != m_valuesExpect.end();

	if (isPathFromRoot("types", "type", "levels"))
		return m_levelsExpect
			.find(element.qName) != m_valuesExpect.end();

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
