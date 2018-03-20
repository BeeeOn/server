#include <Poco/Logger.h>
#include <Poco/NumberParser.h>
#include <Poco/SAX/Attributes.h>

#include "util/BitmapsSAXHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

BitmapsSAXHandler::BitmapsSAXHandler():
	m_flagIndex(-1),
	m_flagInversed(false),
	m_flagAttention(TypeInfo::Level::NONE),
	m_flagsDefaultAttention(TypeInfo::Level::NONE)
{
}

void BitmapsSAXHandler::startElement(
		const SAXElement &element,
		const Attributes &attrList)
{
	if (isPathFromRoot("bitmaps", "bitmap")) {
		XMLString id;
		if (!getAndTrimAttribute(attrList, "id", id))
			error("missing attribute id");

		if (id.empty())
			error("id value is empty");

		m_id = SubtypeInfoID::parse(id);
		m_flagInversed = false;
		m_flagIndex = -1;
		m_flagAttention = TypeInfo::Level::NONE;
		m_flagsDefaultAttention = TypeInfo::Level::NONE;
		m_flags.clear();
		m_groups.clear();
	}
	else if (isPathFromRoot("bitmaps", "bitmap", "flags")) {
		XMLString attention;
		if (getAndTrimAttribute(attrList, "attention", attention)) {
			m_flagsDefaultAttention =
				TypeInfo::Level::parseAttention(attention);
		}
	}
	else if (isPathFromRoot("bitmaps", "bitmap", "flags", "flag")) {
		int index;
		if (!getAttributeAsInt(attrList, "index", index))
			error("missing attribute index");

		m_flagIndex = index;

		XMLString active;
		if (!getAndTrimAttribute(attrList, "active", active))
			active = "high";

		m_flagInversed = (active == "low");

		XMLString attention;
		if (getAndTrimAttribute(attrList, "attention", attention)) {
			m_flagAttention =
				TypeInfo::Level::parseAttention(attention);
		}
		else {
			m_flagAttention = m_flagsDefaultAttention;
		}
	}
	else if (isPathFromRoot("bitmaps", "bitmap", "group")) {
		XMLString label;

		if (!getAndTrimAttribute(attrList, "label", label))
			error("missing attribute label");

		m_group.setName(label);
		m_group.setBits({});
	}
	else if (isPathFromRoot("bitmaps", "bitmap", "group", "bit")) {
		int index;

		if (!getAttributeAsInt(attrList, "index", index))
			error("missing attribute index");

		m_group.addBit(index);
	}
}

void BitmapsSAXHandler::endElement(const SAXElement &element)
{
	if (isPathFromRoot("bitmaps", "bitmap", "group")) {
		m_groups.emplace(m_group);
	}
	else if (isPathFromRoot("bitmaps", "bitmap", "flags", "flag")) {
		if (element.content.empty())
			error("missing content inside tag flag");

		BitmapInfo::Flag flag;
		flag.setName(element.content);
		flag.setIndex(m_flagIndex);
		flag.setInversed(m_flagInversed);
		flag.setAttention(m_flagAttention);

		auto result = m_flags.emplace(flag.index(), flag);
		if (!result.second)
			error("duplicate flag index: " + to_string(flag.index()));
	}
	else if (isPathFromRoot("bitmaps", "bitmap")) {
		m_bitmap.setFlags(m_flags);
		m_bitmap.setGroups(m_groups);
		m_result.emplace(SubtypeInfo(m_id, m_bitmap));
	}
}

bool BitmapsSAXHandler::expectElement(const SAXElement &element) const
{
	if (current().isRoot())
		return element.qName == "bitmaps";

	if (isPathFromRoot("bitmaps"))
		return element.qName == "bitmap";

	if (isPathFromRoot("bitmaps", "bitmap"))
		return element.qName == "flags" || element.qName == "group";

	if (isPathFromRoot("bitmaps", "bitmap", "flags"))
		return element.qName == "flag";

	if (isPathFromRoot("bitmaps", "bitmap", "group"))
		return element.qName == "bit";

	return false;
}

bool BitmapsSAXHandler::expectContent(int length) const
{
	if (current().qName == "flag")
		return true;

	return length == 0;
}

void BitmapsSAXHandler::preprocessContent(
		const XMLChar c[], int &start, int &length)
{
	trimContent(c, start, length);
}
