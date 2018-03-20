#include <cmath>

#include <Poco/Logger.h>
#include <Poco/NumberParser.h>
#include <Poco/SAX/Attributes.h>

#include "util/EnumsSAXHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

EnumsSAXHandler::EnumsSAXHandler()
{
}

EnumsSAXHandler::~EnumsSAXHandler()
{
}

void EnumsSAXHandler::startElement(
		const SAXElement &element,
		const Attributes &attrList)
{
	if (logger().trace()) {
		logger().trace(string(__func__) + ": " + element.qName,
				__FILE__, __LINE__);
	}

	if (isPathFromRoot("enums", "enum")) {
		XMLString id;
		if (!getAndTrimAttribute(attrList, "id", id))
			error("missing attribute id");

		if (id.empty())
			error("id value is empty");

		if (logger().debug())
			logger().debug("parsing type of @id " + id, __FILE__, __LINE__);

		m_id = SubtypeInfoID::parse(id);
		m_temp.setValues({});
	}

	if (isPathFromRoot("enums", "enum", "values", "value")) {
		int equals;
		if (!getAttributeAsInt(attrList, "equals", equals))
			error("missing attribute equals");

		if (!m_values.emplace(equals, "").second)
			error("duplicate value with equals to " + to_string(equals));

		m_lastEquals = equals;

		if (logger().debug()) {
			logger().debug("parsed value equals to " + to_string(m_lastEquals),
					__FILE__, __LINE__);
		}
	}

	if (isPathFromRoot("enums", "enum", "levels", "level")) {
		int equals;
		if (!getAttributeAsInt(attrList, "equals", equals))
			error("missing attribute equals");

		m_level = EnumInfo::Level(equals);

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

		if (logger().debug()) {
			logger().debug("parsed level equals to " + to_string(equals),
					__FILE__, __LINE__);
		}
	}
}

void EnumsSAXHandler::endElement(const SAXElement &element)
{
	if (logger().trace()) {
		logger().trace(string(__func__) + ": " + element.qName,
				__FILE__, __LINE__);
	}

	if (isPathFromRoot("enums", "enum", "values", "value")) {
		m_values[m_lastEquals] = element.content;

		if (logger().debug()) {
			logger().debug("parsed value "
				+ to_string(m_lastEquals) + ": " + element.content,
				__FILE__, __LINE__);
		}
	}

	if (isPathFromRoot("enums", "enum", "values")) {
		m_temp.setValues(m_values);
		m_values.clear();

		if (logger().debug()) {
			logger().debug("parsed values "
					+ to_string(m_temp.values().size()),
					__FILE__, __LINE__);
		}
	}

	if (isPathFromRoot("enums", "enum", "levels", "level")) {
		if (!element.content.empty())
			m_level.setLabel(element.content);

		if (!m_levels.emplace(m_level).second)
			error("overlapping level: " + to_string(m_level.min()));

		if (logger().debug()) {
			logger().debug("parsed level "
					+ to_string(m_level.min()) + ": "
					+ element.content,
					__FILE__, __LINE__);
		}
	}

	if (isPathFromRoot("enums", "enum", "levels")) {
		m_temp.setLevels(m_levels);
		m_levels.clear();

		if (logger().debug()) {
			logger().debug("parsed levels " + to_string(m_temp.levels().size()),
					__FILE__, __LINE__);
		}
	}

	if (isPathFromRoot("enums", "enum")) {
		const SubtypeInfo info(m_id, m_temp);

		if (m_result.find(info) != m_result.end())
			error("enum of id " + m_id.toString() + " already exists");

		if (logger().debug())
			logger().debug("inserting type " + info, __FILE__, __LINE__);

		m_result.insert(info);
	}
}

bool EnumsSAXHandler::expectElement(const SAXElement &element) const
{
	if (current().isRoot())
		return element.qName == "enums";

	if (isPathFromRoot("enums"))
		return element.qName == "enum";

	if (isPathFromRoot("enums", "enum"))
		return element.qName == "values" || element.qName == "levels";

	if (isPathFromRoot("enums", "enum", "values"))
		return element.qName == "value";

	if (isPathFromRoot("enums", "enum", "levels"))
		return element.qName == "level";

	return false;
}

bool EnumsSAXHandler::expectContent(int length) const
{
	if (current().qName == "value")
		return true;

	if (current().qName == "level")
		return true;

	return length == 0;
}

void EnumsSAXHandler::preprocessContent(const XMLChar c[], int &start, int &length)
{
	trimContent(c, start, length);
}
